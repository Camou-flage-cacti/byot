#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xstatus.h"
#include "xaxidma.h"
#include "xil_mem.h"
#include "util.h"
#include "xintc.h"
#include "constants.h"
#include "sleep.h"


//////////////////////// GLOBALS ////////////////////////


// audio DMA access
static XAxiDma sAxiDma;

// LED colors and controller
u32 *led = (u32*) XPAR_RGB_PWM_0_PWM_AXI_BASEADDR;
const struct color RED =    {0x01ff, 0x0000, 0x0000};
const struct color YELLOW = {0x01ff, 0x01ff, 0x0000};
const struct color GREEN =  {0x0000, 0x01ff, 0x0000};
const struct color BLUE =   {0x0000, 0x0000, 0x01ff};

// change states
#define change_state(state, color) c->drm_state = state; setLED(led, color);
#define set_stopped() change_state(STOPPED, RED)
#define set_working() change_state(WORKING, YELLOW)
#define set_playing() change_state(PLAYING, GREEN)
#define set_paused()  change_state(PAUSED, BLUE)

// shared command channel -- read/write for both PS and PL
volatile cmd_channel *c = (cmd_channel*)SHARED_DDR_BASE;

// internal state store
internal_state __attribute__((section (".ssc.code.buffer"))) local_state;
data_content __attribute__((section (".ssc.data.buffer"))) ssc_data;
ro_data_content __attribute__((section (".ssc.ro.data.buffer"))) ssc_ro_data;


//////////////////////// INTERRUPT HANDLING ////////////////////////


// shared variable between main thread and interrupt processing thread
volatile static int InterruptProcessed = FALSE;
static XIntc InterruptController;

void myISR(void) {
    InterruptProcessed = TRUE;
}

//////////////////////// MAIN ////////////////////////
void query_drm(){
	mb_printf("Dummy: Check if calling procedure is ok \r\n");
}
void load_from_shared_to_local() {
    memcpy(local_state.code, (void*)c->code, CODE_SIZE);
}
void format_SSC_input() {

}
void format_SSC_code() {
	ssc_meta_data recived_meta_data;
	unsigned char temp_buffer [24];

	memset(&recived_meta_data, 0, sizeof(ssc_meta_data));

	memcpy(temp_buffer, (void*)c->code , 24);
	get_unsigned_int(temp_buffer, &recived_meta_data);

	memcpy(local_state.code, ((void*)c->code + 24), recived_meta_data.sss_code_size);
	memcpy(ssc_data.data, ((void*)c->code + 24 + recived_meta_data.sss_code_size), recived_meta_data.data_sec_size);
	memcpy(ssc_ro_data.ro_data, ((void*)c->code + 24 + recived_meta_data.sss_code_size + recived_meta_data.data_sec_size), recived_meta_data.ro_data_size);
}
void load_code(){
	mb_printf("Inside Load Code Funciton\r\n");
	int i;
	format_SSC_code();
	mb_printf("-----Read code data-----\r\n");
    i = ((int (*) (void))local_state.code)();
	mb_printf("Code to be executed value returned: '%d'\r\n",  i);
}

int fw_add() {
	return 5 + 3;
}
void forward_to_ssc()
{
	mb_printf("------------------Give execution to SSC----------------");
	((int (*) (void))local_state.code)();
}
void remove_ssc_module(){
	memset(&local_state.code, 0, sizeof(local_state.code));
	memset(&ssc_data, 0, sizeof(data_content));
	memset(&ssc_ro_data, 0, sizeof(ro_data_content));
}
int main() {
    u32 status;

    init_platform();
    microblaze_register_handler((XInterruptHandler)myISR, (void *)0);
    microblaze_enable_interrupts();

    // Initialize the interrupt controller driver so that it is ready to use.
    status = XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Set up the Interrupt System.
    status = SetUpInterruptSystem(&InterruptController, (XInterruptHandler)myISR);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Congigure the DMA
    status = fnConfigDma(&sAxiDma);
    if(status != XST_SUCCESS) {
        mb_printf("DMA configuration ERROR\r\n");
        return XST_FAILURE;
    }

    // Start the LED
    enableLED(led);
    set_stopped();

    // clear command channel
    memset((void*)c, 0, sizeof(cmd_channel));

    mb_printf("--Audio DRM Module has Booted--\n\r");
    fw_add();
    // Handle commands forever
    while(1) {
        // wait for interrupt to start
        if (InterruptProcessed) {
            InterruptProcessed = FALSE;
            set_working();

            // c->cmd is set by the miPod player
            switch (c->cmd) {
            case LOAD_CODE:
            	load_code();
                break;
            case QUERY_DRM:
            	query_drm();
            	break;
            case SSC_COMMAND:
            	forward_to_ssc();
            	break;
            case EXIT:
            	remove_ssc_module();
            	break;
            default:
                break;
            }
            usleep(500);
            set_stopped();
        }
    }

    cleanup_platform();
    return 0;
}
