/*
 * miPod.h
 *
 *  Created on: Jan 9, 2020
 *      Author: ectf
 */

#ifndef SRC_MIPOD_H_
#define SRC_MIPOD_H_


// miPod constants
#define USR_CMD_SZ 64

#define CODE_SIZE 50000
#define INPUT_SIZE 2000
#define OUTPUT_SIZE 2000
#define USERNAME_SZ 64
#define MAX_PIN_SZ 64
#define COMMAND_SIZE 10
// printing utility
#define MP_PROMPT "mP> "
#define mp_printf(...) printf(MP_PROMPT __VA_ARGS__)
#define SHARED_DDR_BASE (0x20000000 + 0x1CC00000 + 0xcf08)
#define USER_PROMPT "miPod %s# "
#define print_prompt() printf(USER_PROMPT, "")
#define print_prompt_msg(...) printf(USER_PROMPT, __VA_ARGS__)

// shared buffer values
//enum commands { QUERY_PLAYER, QUERY_SONG, LOGIN, LOGOUT, SHARE, PLAY, STOP, DIGITAL_OUT, PAUSE, RESTART, FF, RW };
enum commands { LOAD_CODE, QUERY_DRM, SSC_COMMAND};
enum states   { STOPPED, WORKING, PLAYING, PAUSED };

typedef struct __attribute__((__packed__)) {
    char md_size;
    char owner_id;
    char num_regions;
    char num_users;
    char buf[];
} drm_md;

typedef struct __attribute__((__packed__)) {
    char packing1[4];
    int file_size;
    char packing2[32];
    int wav_size;
    drm_md md;
} song;
typedef volatile struct __attribute__((__packed__)) {
    char cmd;                   // from commands enum
    char drm_state;             // from states enum
    char login_status;          // 0 = logged off, 1 = logged on
    char padding;               // not used
    // shared buffer is either a drm song or a query
    union {
        song song;
       // query query;
       // char buf[MAX_SONG_SZ]; // sets correct size of cmd_channel for allocation
    };
} drm_audio_channel;
// struct to interpret shared command channel
typedef volatile struct __attribute__((__packed__)) {
   char cmd;
   char drm_state;
   char code [CODE_SIZE];
   char input[INPUT_SIZE];
   drm_audio_channel drm_chnl;
} cmd_channel;


#endif /* SRC_MIPOD_H_ */
