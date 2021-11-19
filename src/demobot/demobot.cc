#include <extdll.h>
#include <meta_api.h>
#include <dllapi.h>
#include <sdk_util.h>
#include <h_export.h>
#include "demobot.h"
#include "cstrike.h"
#include "wrapper.h"

#include "hldemo.h"

using namespace scale;

inline bool is_alive(edict_t *ent)
{
    if(FNullEnt(ent))
    {
        return false;
    }
    return ent->v.deadflag == DEAD_NO && ent->v.health > 0 && ent->v.movetype != MOVETYPE_NOCLIP;
}

//navigation empty;
demobot::demobot(const char *name) //: _nav(empty)
{
    _player = g_engfuncs.pfnCreateFakeClient(name);
    if (_player->pvPrivateData != nullptr)
    {
        g_engfuncs.pfnFreeEntPrivateData(_player);
        CALL_GAME_ENTITY(PLID, "player", &_player->v);
    }
    _player->pvPrivateData = nullptr;
    _player->v.frags = 0;

    _pev = &_player->v;
    _next_think = 0.0f;
    _think_period = 0.01f;
    _state = demobot_state::INACTIVE;
}
// demobot::demobot(const char *name) //: _nav(empty)
// {
//     // _player = mm_CreateFakeClient(name);
//     // _pev = &_player->v;
//     // _next_think = 0.0f;
//     // _think_period = 0.01f;
//     // _state = demobot_state::INACTIVE;
// }

// demobot::demobot(const std::string &name, const std::string& nav) : _nav(nav)
// {
//     _player = mm_CreateFakeClient(name.c_str());
//     _pev = &_player->v;
//     _next_think = 0.0f;
//     _think_period = 0.01f;
//     _state = demobot_state::INACTIVE;
// }

// demobot::demobot(const std::string &name, navigation &nav) : _nav(nav)
// {
//     _player = mm_CreateFakeClient(name.c_str());
//     _pev = &_player->v;
//     _next_think = 0.0f;
//     _think_period = 0.01f;
//     _state = demobot_state::INACTIVE;
// }

void demobot::load_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if(file == nullptr)
    {
        return;
    }

    // demo header
    hldemo_header h;
    fread(&h, sizeof(h), 1, file);
    // _map = std::string((char *)h.map_name);
    fseek(file, h.directory_entry_offset, SEEK_SET);

    // demo directory entries
    uint32_t count;
    fread(&count, sizeof(count), 1, file);
    hldemo_directory_entry e;
    for (int i = 0; i < count; i++)
    {
        fread(&e, sizeof(e), 1, file);
    }
    // _name = std::string((char *)e.description);
    fseek(file, e.data_offset, SEEK_SET);

    hldemo_frame_header fh;
    bool valid = true;
    while (valid)
    {
        fread(&fh, sizeof(fh), 1, file);
        // printf(
        //     "[Frame] TYPE: %u TIME: %f\n",
        //     fh.type, fh.timestamp
        // );
        switch (fh.type)
        {
        case 0:
        {
            break;
        }
        case 1:
        {
            hldemo_frame_status d;
            fread(&d, sizeof(d), 1, file);
            fseek(file, d.tail_length, SEEK_CUR);
            _nav.push_back({d.timestamp, d.buttons, d.view_angles, d.velocity, d.origin});
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            hldemo_frame_concmd d;
            fread(&d, sizeof(d), 1, file);
            break;
        }
        case 4:
        {
            hldemo_frame_weapon d;
            fread(&d, sizeof(d), 1, file);
            break;
        }
        case 5:
        {
            valid = false;
            break;
        }
        case 6:
        {
            hldemo_frame_6 d;
            fread(&d, sizeof(d), 1, file);
            break;
        }
        case 7:
        {
            hldemo_frame_7 d;
            fread(&d, sizeof(d), 1, file);
            break;
        }
        case 8:
        {
            hldemo_frame_8 d;
            fread(&d, sizeof(d), 1, file);
            fseek(file, d.length, SEEK_CUR);
            fseek(file, 16, SEEK_CUR);
            break;
        }
        case 9:
        {
            hldemo_frame_9 d;
            fread(&d, sizeof(d), 1, file);
            fseek(file, d.length, SEEK_CUR);
            break;
        }
        default:
        {
            valid = false;
            break;
        }
        }
    }
    fclose(file);
    printf("[DEMOBOT] Loaded %s with %u navs.\n", filename, _nav.size());
    printf("[DEMOBOT] Demo map name: %s description: %s.\n", h.map_name, e.description);
    _nav_current = 0;
}

void demobot::think()
{
    float time = globals->time;
    _pev->flags |= FL_FAKECLIENT;
    demobot_state old_state = _state;
    if (time >= _next_think)
    {
        switch (_state)
        {
            case demobot_state::INACTIVE:
            {
                // if(_player->)
                char reject[256] = { 0, };
                char ip[20] = { 0, };
                sprintf(ip, "127.0.0.%d", 100 + rand() % 20);
                MDLL_ClientConnect(_player, "test", ip, reject);
                auto buffer = g_engfuncs.pfnGetInfoKeyBuffer(_player);
                g_engfuncs.pfnSetClientKeyValue(g_engfuncs.pfnIndexOfEdict(_player), buffer, "_vgui_menus", "0");
                MDLL_ClientPutInServer(_player);
                _player->v.flags |= FL_FAKECLIENT;
                _state = demobot_state::CONNECTED;
                break;
            }
            case demobot_state::CONNECTED:
            {
                if (!is_alive(_player))
                {
                    _state = demobot_state::SELECT_TEAM;
                }
                break;
            }
            case demobot_state::SELECT_TEAM:
            {
                if (_team == demobot_team::T || _team == demobot_team::CT)
                {
                    _state = demobot_state::SPAWNED;
                }
                else
                {
                    *((demobot_team *)_player->pvPrivateData + OFFSET_TEAM) = demobot_team::CT;
                    _state = demobot_state::SELECT_MODEL;
                }
                break;
            }
            case demobot_state::SELECT_MODEL:
            {
                // TO-DO: set _player->pvPrivateData + OFFSET_INTERNALMODEL ...
                _state = demobot_state::READY;
                break;
            }
            case demobot_state::READY:
            {
                MDLL_Spawn(_player);
                _state = demobot_state::SPAWNED;
                break;
            }
            case demobot_state::SPAWNED:
            {
                if(has_motion())
                {
                    const auto &motion = next_motion();
                    move(motion.button, motion.view_angle, motion.velocity, motion.origin);
                    _pev->origin = motion.origin;
                    _pev->velocity = motion.velocity;
                    _pev->angles.x = motion.view_angle.x / (-3.0f);
                    _pev->angles.y = motion.view_angle.y;
                    _pev->angles.z = motion.view_angle.z;
                    _pev->button = motion.button;
                    if (_pev->gaitsequence == 4 && (~_pev->flags & FL_ONGROUND))
                    {
                        _pev->gaitsequence == 6;
                    }
                }
                break;
            }
        }
    }
    demobot_state new_state = _state;
    if(new_state != old_state)
    {
        printf("state change: %d -> %d\n", old_state, new_state);
    }
    _next_think = time + _think_period;
}

const motion_state demobot::next_motion()
{
    if(_nav_current >= _nav.size())
    {
        _nav_current = 0;
    }
    return _nav[_nav_current++];
}

bool demobot::has_motion()
{
    return _nav.size() > 0;
}

void demobot::update()
{
    if(_state == demobot_state::INACTIVE)
    {
        return;
    }
    _team = *((demobot_team *)_player->pvPrivateData + OFFSET_TEAM);
}

// void demobot::command(const std::string& str)
// {
//     MDLL_ClientCommand(_player);
// }

void demobot::move(uint16_t buttons, vec3_t view_angle, vec3_t velocity, vec3_t origin)
{
    g_engfuncs.pfnRunPlayerMove(_player, &view_angle.x, velocity.x, velocity.y, 0.0f, buttons, 0, int(_think_period * 1000));
}

void demobot::stale()
{
    _state = demobot_state::INACTIVE;
}