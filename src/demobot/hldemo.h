#pragma once

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

struct hldemo_frame_status
{
    float timestamp;
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