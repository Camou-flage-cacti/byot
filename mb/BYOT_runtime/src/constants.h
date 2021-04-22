#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include "xil_printf.h"

// shared DDR address
#define SHARED_DDR_BASE (0x20000000 + 0x1CC00000)

// printing utility
#define MB_PROMPT "\r\nMB> "
#define mb_printf(...) xil_printf(MB_PROMPT __VA_ARGS__)

// protocol constants
#define CODE_SIZE 50000


// LED colors and controller
struct color {
    u32 r;
    u32 g;
    u32 b;
};


enum commands { LOAD_CODE, QUERY_DRM, QUERY_PLAYER, QUERY_SONG, LOGIN, LOGOUT, SHARE, PLAY, STOP, DIGITAL_OUT, PAUSE, RESTART, FF, RW };
enum states   { STOPPED, WORKING, PLAYING, PAUSED };


// struct to interpret shared command channel

typedef volatile struct __attribute__((__packed__)) {
   char cmd;
   char drm_state;
   char code [CODE_SIZE];
   //char input[INPUT_SIZE];
   //char output [OUTPUT_size];
} cmd_channel;

// store of internal state

typedef struct {
    char code [CODE_SIZE];
    //char input[INPUT_SIZE];
    //char output[OUTPUT_size];
} internal_state;
#endif /* SRC_CONSTANTS_H_ */

