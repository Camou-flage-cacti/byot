#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xstatus.h"
#include "xaxidma.h"
#include "xil_mem.h"
#include "util.h"
#include "secrets.h"
#include "xintc.h"
#include "constants.h"
#include "sleep.h"
#include "BYOT_header.h"
//////////////////////// GLOBALS ////////////////////////

// audio DMA access
static XAxiDma sAxiDma;

// LED colors and controller
u32 *led = (u32 *)XPAR_RGB_PWM_0_PWM_AXI_BASEADDR;
const struct color RED = {0x01ff, 0x0000, 0x0000};
const struct color YELLOW = {0x01ff, 0x01ff, 0x0000};
const struct color GREEN = {0x0000, 0x01ff, 0x0000};
const struct color BLUE = {0x0000, 0x0000, 0x01ff};

// change states
#define change_state(state, color) \
    c->drm_state = state;          \
    setLED(led, color);
#define set_stopped() change_state(STOPPED, RED)
#define set_working() change_state(WORKING, YELLOW)
#define set_playing() change_state(PLAYING, GREEN)
#define set_paused() change_state(PAUSED, BLUE)

// shared command channel -- read/write for both PS and PL
player_input received_input;
volatile char *input = (char *)0x189f8;
// internal state store
drm_internal_state s;

int init = 0;
//////////////////////// INTERRUPT HANDLING ////////////////////////

// shared variable between main thread and interrupt processing thread
volatile static int InterruptProcessed = FALSE;
static XIntc InterruptController;

void myISR(void)
{
    InterruptProcessed = TRUE;
}


int dummy()
{
    usleep(500);
    init_platform();
    cleanup_platform();
}
//////////////////////// UTILITY FUNCTIONS ////////////////////////

// returns whether an rid has been provisioned
int is_provisioned_rid(char rid)
{
    for (int i = 0; i < NUM_PROVISIONED_REGIONS; i++)
    {
        if (rid == PROVISIONED_RIDS[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

// looks up the region name corresponding to the rid
int rid_to_region_name(char rid, char **region_name, int provisioned_only)
{
    for (int i = 0; i < NUM_REGIONS; i++)
    {
        if (rid == REGION_IDS[i] &&
            (!provisioned_only || is_provisioned_rid(rid)))
        {
            *region_name = (char *)REGION_NAMES[i];
            return TRUE;
        }
    }

    mb_printf("Could not find region ID '%d'\r\n", rid);
    *region_name = "<unknown region>";
    return FALSE;
}

// looks up the rid corresponding to the region name
int region_name_to_rid(char *region_name, char *rid, int provisioned_only)
{
    for (int i = 0; i < NUM_REGIONS; i++)
    {
        if (!strcmp(region_name, REGION_NAMES[i]) &&
            (!provisioned_only || is_provisioned_rid(REGION_IDS[i])))
        {
            *rid = REGION_IDS[i];
            return TRUE;
        }
    }

    mb_printf("Could not find region name '%s'\r\n", region_name);
    *rid = -1;
    return FALSE;
}

// returns whether a uid has been provisioned
int is_provisioned_uid(char uid)
{
    for (int i = 0; i < NUM_PROVISIONED_USERS; i++)
    {
        if (uid == PROVISIONED_UIDS[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

// looks up the username corresponding to the uid
int uid_to_username(char uid, char **username, int provisioned_only)
{
    for (int i = 0; i < NUM_USERS; i++)
    {
        if (uid == USER_IDS[i] &&
            (!provisioned_only || is_provisioned_uid(uid)))
        {
            *username = (char *)USERNAMES[i];
            return TRUE;
        }
    }

    mb_printf("Could not find uid '%d'\r\n", uid);
    *username = "<unknown user>";
    return FALSE;
}

// looks up the uid corresponding to the username
int username_to_uid(char *username, char *uid, int provisioned_only)
{
    for (int i = 0; i < NUM_USERS; i++)
    {
        if (!strcmp(username, USERNAMES[USER_IDS[i]]) &&
            (!provisioned_only || is_provisioned_uid(USER_IDS[i])))
        {
            *uid = USER_IDS[i];
            return TRUE;
        }
    }

    mb_printf("Could not find username '%s'\r\n", username);
    *uid = -1;
    return FALSE;
}

// checks if the song loaded into the shared buffer is locked for the current user
int is_locked()
{
    int locked = TRUE;

    // check for authorized user
    if (!s.logged_in)
    {
        mb_printf("No user logged in");
    }
    else
    {
        load_song_md();

        // check if user is authorized to play song
        if (s.uid == s.song_md.owner_id)
        {
            locked = FALSE;
        }
        else
        {
            for (int i = 0; i < NUM_PROVISIONED_USERS && locked; i++)
            {
                if (s.uid == s.song_md.uids[i])
                {
                    locked = FALSE;
                }
            }
        }

        if (locked)
        {
            mb_printf("User '%s' does not have access to this song", s.username);
            return locked;
        }
        mb_printf("User '%s' has access to this song", s.username);
        locked = TRUE; // reset lock for region check

        // search for region match
        for (int i = 0; i < s.song_md.num_regions; i++)
        {
            for (int j = 0; j < (u8)NUM_PROVISIONED_REGIONS; j++)
            {
                if (PROVISIONED_RIDS[j] == s.song_md.rids[i])
                {
                    locked = FALSE;
                }
            }
        }

        if (!locked)
        {
            mb_printf("Region Match. Full Song can be played. Unlocking...");
        }
        else
        {
            mb_printf("Invalid region");
        }
    }
    return locked;
}

int gen_song_md(char *buf)
{
    buf[0] = ((5 + s.song_md.num_regions + s.song_md.num_users) / 2) * 2; // account for parity
    buf[1] = s.song_md.owner_id;
    buf[2] = s.song_md.num_regions;
    buf[3] = s.song_md.num_users;
    memcpy(buf + 4, s.song_md.rids, s.song_md.num_regions);
    memcpy(buf + 4 + s.song_md.num_regions, s.song_md.uids, s.song_md.num_users);

    return buf[0];
}
/*
void login()
{
    if (s.logged_in)
    {
        mb_printf("Already logged in. Please log out first.\r\n");
        memcpy((void *)c->username, s.username, USERNAME_SZ);
        memcpy((void *)c->pin, s.pin, MAX_PIN_SZ);
    }
    else
    {
        for (int i = 0; i < NUM_PROVISIONED_USERS; i++)
        {
            // search for matching username
            if (!strcmp((void *)c->username, USERNAMES[PROVISIONED_UIDS[i]]))
            {
                // check if pin matches
                if (!strcmp((void *)c->pin, PROVISIONED_PINS[i]))
                {
                    //update states
                    s.logged_in = 1;
                    c->login_status = 1;
                    memcpy(s.username, (void *)c->username, USERNAME_SZ);
                    memcpy(s.pin, (void *)c->pin, MAX_PIN_SZ);
                    s.uid = PROVISIONED_UIDS[i];
                    mb_printf("Logged in for user '%s'\r\n", c->username);
                    return;
                }
                else
                {
                    // reject login attempt
                    mb_printf("Incorrect pin for user '%s'\r\n", c->username);
                    memset((void *)c->username, 0, USERNAME_SZ);
                    memset((void *)c->pin, 0, MAX_PIN_SZ);
                    return;
                }
            }
        }

        // reject login attempt
        mb_printf("User not found\r\n");
        memset((void *)c->username, 0, USERNAME_SZ);
        memset((void *)c->pin, 0, MAX_PIN_SZ);
    }
}

// attempt to log out
void logout()
{
    if (c->login_status)
    {
        mb_printf("Logging out...\r\n");
        s.logged_in = 0;
        c->login_status = 0;
        memset((void *)c->username, 0, USERNAME_SZ);
        memset((void *)c->pin, 0, MAX_PIN_SZ);
        s.uid = 0;
    }
    else
    {
        mb_printf("Not logged in\r\n");
    }
}
*/

//////////////////////// MAIN ////////////////////////

int main()
{
	if (init == 0) {
		init++;
		xil_printf("DRM AUDIO SSC LOADED");
		return 0;
	}
	memcpy(&received_input, (void *)input, sizeof(player_input));
	if (!strcmp(received_input.cmd, "login")) {
		xil_printf("LOGIN COMMAND received");
	}
    return 0;
}
