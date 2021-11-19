#pragma once
#include <extdll.h>
#include <vector>
#include <string>
#include <cstdint>

namespace scale
{

    // #pragma pack(push, 1)
    //     struct hldemo_header
    //     {
    //         uint8_t magic[8];
    //         uint32_t demo_protocol;
    //         uint32_t game_protocol;
    //         uint8_t map_name[260];
    //         uint8_t game_directory[260];
    //         uint32_t crc;
    //         uint32_t directory_entry_offset;
    //     };

    //     struct hldemo_directory_entry
    //     {
    //         uint32_t type;
    //         uint8_t description[64];
    //         uint32_t flags;
    //         uint32_t cdtrack;
    //         uint32_t time;
    //         uint32_t frame_count;
    //         uint32_t data_offset;
    //         uint32_t data_size;
    //     };

    //     struct hldemo_frame_header
    //     {
    //         uint8_t type;
    //         float timestamp;
    //         uint32_t count;
    //     };

    //     struct hldemo_frame_status
    //     {
    //         float timestamp;
    //         float view_origin[3];
    //         float view_angles[3];
    //         uint8_t _[64];
    //         float velocity[3];
    //         float origin[3];
    //         uint8_t __[124];
    //         float _usercmd_view_angles[3];
    //         uint32_t forward_move;
    //         uint32_t side_move;
    //         uint32_t up_move;
    //         uint8_t light_level;
    //         uint8_t ___;
    //         uint16_t buttons;
    //         uint8_t ____[196];
    //         uint32_t tail_length;
    //         uint8_t tail_data[0];
    //     };

    //     struct hldemo_frame_2
    //     {
    //     };

    //     struct hldemo_frame_concmd
    //     {
    //         uint8_t command[64];
    //     };

    //     struct hldemo_frame_weapon
    //     {
    //         float origin[3];
    //         float view_angles[3];
    //         uint32_t weapon;
    //         uint32_t fov;
    //     };

    //     struct hldemo_frame_5
    //     {
    //     };

    //     struct hldemo_frame_6
    //     {
    //         uint32_t flags;
    //         uint32_t index;
    //         uint32_t delay;
    //         uint32_t event_flags;
    //         uint32_t entity_index;
    //         float origin[3];
    //         float angles[3];
    //         float velocity[3];
    //         float ducking;
    //         float fparam1;
    //         float fparam2;
    //         uint32_t iparam1;
    //         uint32_t iparam2;
    //         uint32_t bparam1;
    //         uint32_t bparam2;
    //     };

    //     struct hldemo_frame_7
    //     {
    //         uint8_t _[8];
    //     };

    //     struct hldemo_frame_8
    //     {
    //         uint32_t _;
    //         uint32_t length;
    //         uint8_t message[0];
    //         // uint8_t _[16];
    //     };

    //     struct hldemo_frame_9
    //     {
    //         uint32_t length;
    //         uint8_t message[0];
    //     };

    // #pragma pack(pop)

    //     struct motion_state
    //     {
    //         float time;
    //         uint16_t button;
    //         vec3_t view_angle;
    //         vec3_t velocity;
    //         vec3_t origin;
    //     };
    //     class navigation final
    //     {
    //     private:
    //         std::vector<motion_state> _nav;
    //         size_t _current;
    //         std::string _map;
    //         std::string _name;
    //         float _time;
    //         void _load_dem(const std::string& filename)
    //         {
    //             _current = 0;
    //             FILE *file = fopen(filename.c_str(), "rb");

    //             // demo header
    //             hldemo_header h;
    //             fread(&h, sizeof(h), 1, file);
    //             _map = std::string((char *) h.map_name);
    //             fseek(file, h.directory_entry_offset, SEEK_SET);

    //             // demo directory entries
    //             uint32_t count;
    //             fread(&count, sizeof(count), 1, file);
    //             hldemo_directory_entry e;
    //             for (int i = 0; i < count; i++)
    //             {
    //                 fread(&e, sizeof(e), 1, file);
    //             }
    //             _name = std::string((char *) e.description);
    //             fseek(file, e.data_offset, SEEK_SET);

    //             //
    //             hldemo_frame_header fh;
    //             bool valid = true;
    //             while(valid)
    //             {
    //                 fread(&fh, sizeof(fh), 1, file);
    //                 switch(fh.type)
    //                 {
    //                     case 0:
    //                     {
    //                         break;
    //                     }
    //                     case 1:
    //                     {
    //                         hldemo_frame_status d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         _nav.push_back({d.timestamp, d.buttons, d.view_angles, d.velocity, d.origin});
    //                         break;
    //                     }
    //                     case 2:
    //                     {
    //                         break;
    //                     }
    //                     case 3:
    //                     {
    //                         hldemo_frame_concmd d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         break;
    //                     }
    //                     case 4:
    //                     {
    //                         hldemo_frame_weapon d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         break;
    //                     }
    //                     case 5:
    //                     {
    //                         valid = false;
    //                         break;
    //                     }
    //                     case 6:
    //                     {
    //                         hldemo_frame_6 d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         break;
    //                     }
    //                     case 7:
    //                     {
    //                         hldemo_frame_7 d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         break;
    //                     }
    //                     case 8:
    //                     {
    //                         hldemo_frame_8 d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         fseek(file, d.length, SEEK_CUR);
    //                         fseek(file, 16, SEEK_CUR);
    //                         break;
    //                     }
    //                     case 9:
    //                     {
    //                         hldemo_frame_9 d;
    //                         fread(&d, sizeof(d), 1, file);
    //                         fseek(file, d.length, SEEK_CUR);
    //                         break;
    //                     }
    //                     default:
    //                     {
    //                         valid = false;
    //                         break;
    //                     }
    //                 }
    //             }
    //             fclose(file);
    //         }
    //         void _load_ynv(const std::string &filename)
    //         {
    //             FILE *file = fopen(filename.c_str(), "rb");
    //         }

    //     public:
    //         navigation()
    //         {
    //         }
    //         navigation(const std::string &filename) : _current(0)
    //         {
    //             load_file(filename);
    //         }
    //         bool valid() const
    //         {
    //             return _nav.size() > 0;
    //         }
    //         void load_file(const std::string &filename)
    //         {
    //             _nav.clear();
    //             _nav.shrink_to_fit();
    //             std::string ext = filename.substr(filename.size() - 4, 4);
    //             printf("Loading file %s", filename.c_str());
    //             if(ext == ".dem")
    //             {
    //                 _load_dem(filename);
    //             }
    //             else if(ext == ".ynv")
    //             {
    //                 _load_ynv(filename);
    //             }
    //         }
    //         const motion_state &next()
    //         {
    //             if (_current == _nav.size())
    //             {
    //                 _current = 0;
    //             }
    //             return _nav[_current++];
    //         }
    //         // merge the following n states into 1 state
    //         const motion_state next(size_t n)
    //         {
    //             motion_state result = {0.0f, 0, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    //             while(n-- && _current < _nav.size())
    //             {
    //                 motion_state state = next();
    //                 result.button |= state.button;
    //                 if(n == 0 || _current == _nav.size())
    //                 {
    //                     result.origin = state.origin;
    //                     result.view_angle = state.view_angle;
    //                     result.velocity = state.velocity;
    //                 }
    //             }
    //             return result;
    //         }
    //     };
    enum demobot_state
    {
        INACTIVE,
        CONNECTED,
        SELECT_TEAM,
        SELECT_MODEL,
        READY,
        SPAWNED,
    };
    enum demobot_team
    {
        UNASSIGNED,
        T,
        CT,
        SPECTATOR
    };

    struct motion_state
    {
        float time;
        uint16_t button;
        vec3_t view_angle;
        vec3_t velocity;
        vec3_t origin;
    };

    class demobot final
    {
    private:
        int _index;
        edict_t *_player;
        entvars_t *_pev;
        float _next_think;
        float _think_period;
        demobot_state _state;
        demobot_team _team;

        std::vector<motion_state> _nav;
        size_t _nav_current;
        // navigation& _nav;

    public:
        // friend class demobot_manager;
        // demobot(const std::string& name);
        demobot(const char *name);
        // ~demobot() = default;
        // demobot(const std::string& name, const std::string& nav);
        // demobot(const std::string& name, navigation& nav);
        // do action
        void think();
        // sync some basic info
        void update();
        const motion_state next_motion();
        bool has_motion();
        // void command(const std::string& str);
        void move(uint16_t buttons, vec3_t view_angle, vec3_t velocity, vec3_t origin);
        void load_file(const char *filename);
        void stale();
    };

    // class demobot_manager final
    // {
    // private:
    //     std::vector<demobot> _bots;
    // public:
    //     demobot_manager() {};
    //     void add(const demobot& bot);
    //     void destroy();
    // };
};