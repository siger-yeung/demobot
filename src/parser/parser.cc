#include <cstdio>
#include <cstdint>

#pragma pack(push, 1)

struct hldemo_header
{
    uint8_t magic[8];
    uint32_t demo_protocol;
    uint32_t game_protocol;
    uint8_t map_name[260];
    uint8_t game_directory[260];
    uint32_t crc;
    uint32_t directory_entry_offset;
};

struct hldemo_directory_entry
{
    uint32_t type;
    uint8_t description[64];
    uint32_t flags;
    uint32_t cdtrack;
    uint32_t time;
    uint32_t frame_count;
    uint32_t data_offset;
    uint32_t data_size;
};

struct hldemo_frame_header
{
    uint8_t type;
    float timestamp;
    uint32_t count;
};

enum FRAME_TYPE
{
    FRAME_TYPE_0,
    FRAME_TYPE_STATUS,
    FRAME_TYPE_2,
    FRAME_TYPE_CONCMD,
    FRAME_TYPE_WEAPON,
    FRAME_TYPE_5,
    FRAME_TYPE_6,
    FRAME_TYPE_7,
    FRAME_TYPE_8,
    FRAME_TYPE_9
};

struct hldemo_frame_status
{
    uint32_t timestamp;
    float view_origin[3];
    float view_angles[3];
    uint8_t _[64];
    float velocity[3];
    float origin[3];
    uint8_t __[124];
    float _usercmd_view_angles[3];
    uint32_t forward_move;
    uint32_t side_move;
    uint32_t up_move;
    uint8_t light_level;
    uint8_t ___;
    uint16_t buttons;
    uint8_t ____[196];
    uint32_t tail_length;
    uint8_t tail_data[0];
};

struct hldemo_frame_2
{
};

struct hldemo_frame_concmd
{
    uint8_t command[64];
};

struct hldemo_frame_weapon
{
    float origin[3];
    float view_angles[3];
    uint32_t weapon;
    uint32_t fov;
};

struct hldemo_frame_5
{
};

struct hldemo_frame_6
{
    uint32_t flags;
    uint32_t index;
    uint32_t delay;
    uint32_t event_flags;
    uint32_t entity_index;
    float origin[3];
    float angles[3];
    float velocity[3];
    float ducking;
    float fparam1;
    float fparam2;
    uint32_t iparam1;
    uint32_t iparam2;
    uint32_t bparam1;
    uint32_t bparam2;
};

struct hldemo_frame_7
{
    uint8_t _[8];
};

struct hldemo_frame_8
{
    uint32_t _;
    uint32_t length;
    uint8_t message[0];
    // uint8_t _[16];
};

struct hldemo_frame_9
{
    uint32_t length;
    uint8_t message[0];
};

#pragma pack(pop)

void parse_header(FILE *f)
{
    hldemo_header h;
    fread(&h, sizeof(h), 1, f);
    printf(
        "[Demo file header]\n"
        "    Map: %s\n"
        "Gamedir: %s\n"
        " Game v: %d\n"
        " Demo v: %d\n",
        h.map_name,
        h.game_directory,
        h.game_protocol,
        h.demo_protocol
    );
    fseek(f, h.directory_entry_offset, SEEK_SET);
}

void parse_directory(FILE *f)
{
    uint32_t count;
    fread(&count, sizeof(count), 1, f);
    hldemo_directory_entry e;
    for(int i = 0; i < count; i++)
    {
        fread(&e, sizeof(e), 1, f);
        printf(
            "[Demo file directory entry %d]\n"
            "   Time: %u\n"
            "Summary: %s\n"
            " Offset: %u\n"
            "   Size: %u\n",
            i,
            e.time,
            e.description,
            e.data_offset,
            e.data_size
        );
    }
    fseek(f, e.data_offset, SEEK_SET);
}

void parse_frames(FILE *f)
{
    hldemo_frame_header fh;
    bool valid = true;
    while(valid)
    {
        fread(&fh, sizeof(fh), 1, f);
        printf(
            "[Frame] TYPE: %u TIME: %f\n",
            fh.type, fh.timestamp
        );
        switch(fh.type)
        {
            case FRAME_TYPE_0:
            {
                break;
            }
            case FRAME_TYPE_STATUS:
            {
                hldemo_frame_status d;
                fread(&d, sizeof(d), 1, f);
                printf(
                    "[Frame STATUS] %f %d\n"
                    " Origin: %f %f %f\n"
                    "Velocity: %f %f %f\n",
                    fh.timestamp,
                    d.timestamp,
                    d.origin[0], d.origin[1], d.origin[2],
                    d.velocity[0], d.velocity[1], d.velocity[2]
                );
                fseek(f, d.tail_length, SEEK_CUR);
                fprintf(stderr, "{%u, {%.5f,%.5f,%.5f},{%.5f,%.5f,%.5f},{%.5f,%.5f,%.5f}},\n",
                    d.buttons,
                    d.view_angles[0], d.view_angles[1], d.view_angles[2],
                    d.origin[0], d.origin[1], d.origin[2],
                    d.velocity[0], d.velocity[1], d.velocity[2]
                );
                break;
            }
            case FRAME_TYPE_2:
            {
                break;
            }
            case FRAME_TYPE_CONCMD:
            {
                hldemo_frame_concmd d;
                fread(&d, sizeof(d), 1, f);
                break;
            }
            case FRAME_TYPE_WEAPON:
            {
                hldemo_frame_weapon d;
                fread(&d, sizeof(d), 1, f);
                break;
            }
            case FRAME_TYPE_5:
            {
                valid = false;
                break;
            }
            case FRAME_TYPE_6:
            {
                hldemo_frame_6 d;
                fread(&d, sizeof(d), 1, f);
                break;
            }
            case FRAME_TYPE_7:
            {
                hldemo_frame_7 d;
                fread(&d, sizeof(d), 1, f);
                break;
            }
            case FRAME_TYPE_8:
            {
                hldemo_frame_8 d;
                fread(&d, sizeof(d), 1, f);
                fseek(f, d.length, SEEK_CUR);
                fseek(f, 16, SEEK_CUR);
                break;
            }
            case FRAME_TYPE_9:
            {
                hldemo_frame_9 d;
                fread(&d, sizeof(d), 1, f);
                fseek(f, d.length, SEEK_CUR);
                break;
            }
            default:
            {
                valid = false;
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        printf("demoparser [demofile.dem]\n");
    }
    else
    {
        FILE *file = fopen(argv[1], "rb");
        parse_header(file);
        parse_directory(file);
        parse_frames(file);
        fclose(file);
    }
    return 0;
}