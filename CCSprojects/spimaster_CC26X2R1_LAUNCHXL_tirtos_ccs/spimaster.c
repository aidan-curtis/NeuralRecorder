/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* RF settings*/
#include <stdlib.h>

/* TI Drivers */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/* Driverlib Header files */
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

/* Board Header files */
#include "smartrf_settings/smartrf_settings.h"

/* Packet TX Configuration */
#define PAYLOAD_LENGTH      16 // number of channels *2
#define PACKET_INTERVAL     (uint32_t)(4000000*0.001f) /* Set packet interval to 1ms */
//#define PACKET_INTERVAL     (uint32_t)(0.0f) /* Set packet interval to 0ms */


/*
 *  ======== spimaster.c ========
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>

/* Example/Board Header files */
#include "Board.h"

#define THREADSTACKSIZE (1024)

#define SPI_MSG_LENGTH  (1)

#define MAX_LOOP        (40000)

static Display_Handle display;

uint16_t masterRxBuffer[SPI_MSG_LENGTH];
uint16_t masterTxBuffer[SPI_MSG_LENGTH];

int num_messages = 60;
uint16_t messages[60]; // initial messages for setup

/* Start--> RF variable declarations*/
/***** Variable declarations *****/
static RF_Object rfObject;
static RF_Handle rfHandle;

/* Pin driver handle */
static PIN_Handle ledPinHandle; // unused
static PIN_State ledPinState;

static uint8_t packet[PAYLOAD_LENGTH];
//static uint16_t channels[16] = {0b0000000000000000, 0b0000000100000000, 0b0000001000000000, 0b0000001100000000,
//                                0b0000010000000000, 0b0000010100000000, 0b0000011000000000, 0b0000011100000000,
//                                0b0000100000000000, 0b0000100100000000, 0b0000101000000000, 0b0000101100000000,
//                                0b0000110000000000, 0b0000110100000000, 0b0000111000000000, 0b0000111100000000,
//};


// Compression stuff
const int lag = 5;
const int num_channels = 8;
double calculated_betas[] = {-0.36, 0.71, -0.57, -0.65, 1.65}; // reversed coeffs
uint16_t storage_coeffs[num_channels][lag+1];
uint16_t constant_offset = 0;
uint16_t error[num_channels];


void load_error_to_packet(){
    int ch;
    for(ch = 0; ch < num_channels; ch += 1){
        packet[ch*2] = (error[ch] >> 8);
        packet[ch*2+1] = error[ch] & 0xff;
    }
}

void get_error_signal(){
    int ch;
    for(ch = 0; ch < num_channels; ch += 1){
        error[ch] = 0;
        int i;
        for(i = 0; i < lag; i += 1){
            error[ch] += storage_coeffs[ch][i]*calculated_betas[i];
        }
        error[ch] = error[ch]-storage_coeffs[ch][lag];
    }
}

void shift_channels(){
    int ch;
    for(ch = 0; ch < num_channels; ch += 1){
        int i;
        for(i = 1; i <= lag; i+=1){
            storage_coeffs[ch][i-1] = storage_coeffs[ch][i];
        }
    }
}

void store_packet_in_hist(){
    shift_channels();
    int ch;
    for(ch = 0; ch < num_channels; ch += 1){
        storage_coeffs[ch][lag] = (packet[ch*2] << 8) | packet[ch*2+1];
    }
}


//bit13-bit8 are channel numbers
static uint16_t channels[8] = { 0b0000010000000000, 0b0000010100000000, 0b0000011000000000, 0b0000011100000000,
                                0b0000100000000000, 0b0000100100000000, 0b0000101000000000, 0b0000101100000000
};
static uint8_t ind = 0; // packet end indicator

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config pinTable[] =
{
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#if defined Board_CC1352R1_LAUNCHXL
    Board_DIO30_RFSW | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#ifdef POWER_MEASUREMENT
#if defined(Board_CC1350_LAUNCHXL)
    Board_RF_POWER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#endif
    PIN_TERMINATE
};
//static uint16_t seqNumber;
/*RF variable declarations END*/


/* Semaphore to block master until slave is ready for transfer */
sem_t masterSem;

/*
 *  ======== slaveReadyFxn ========
 *  Callback function for the GPIO interrupt on Board_SPI_SLAVE_READY.
 */
void slaveReadyFxn(uint_least8_t index)
{
    sem_post(&masterSem);
}

/*
 *  ======== masterThread ========
 *  Master SPI sends a message to slave while simultaneously receiving a
 *  message from the slave.
 */
void *masterThread(void *arg0)
{
    SPI_Handle      masterSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    uint32_t        i;
    uint32_t        j;
    bool            transferOK;
    int32_t         status;

    // Start --> RF crap
    uint32_t curtime;
    RF_Params rfParams;
    RF_Params_init(&rfParams);
    ledPinHandle = PIN_open(&ledPinState, pinTable);
//    if (ledPinHandle == NULL)
//    {
//        while(1);
//    }
    RF_cmdPropTx.pktLen = PAYLOAD_LENGTH;
    RF_cmdPropTx.pPkt = packet;
    RF_cmdPropTx.startTrigger.triggerType = TRIG_ABSTIME;
    RF_cmdPropTx.startTrigger.pastTrig = 1;
    RF_cmdPropTx.startTime = 0;

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Set the frequency */
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Get current time */
    curtime = RF_getCurrentTime();
    // <-- End RF crap

    /*
     * Board_SPI_MASTER_READY & Board_SPI_SLAVE_READY are GPIO pins connected
     * between the master & slave.  These pins are used to synchronize
     * the master & slave applications via a small 'handshake'.  The pins
     * are later used to synchronize transfers & ensure the master will not
     * start a transfer until the slave is ready.  These pins behave
     * differently between spimaster & spislave examples:
     *
     * spimaster example:
     *     * Board_SPI_MASTER_READY is configured as an output pin.  During the
     *       'handshake' this pin is changed from low to high output.  This
     *       notifies the slave the master is ready to run the application.
     *       Afterwards, the pin is used by the master to notify the slave it
     *       has opened Board_SPI_MASTER.  When Board_SPI_MASTER is opened, this
     *       pin will be pulled low.
     *
     *     * Board_SPI_SLAVE_READY is configured as an input pin. During the
     *       'handshake' this pin is read & a high value will indicate the slave
     *       ready to run the application.  Afterwards, a falling edge interrupt
     *       will be configured on this pin.  When the slave is ready to perform
     *       a transfer, it will pull this pin low.
     *
     * Below we set Board_SPI_MASTER_READY & Board_SPI_SLAVE_READY initial
     * conditions for the 'handshake'.
     */

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.dataSize = 16;
    masterSpi = SPI_open(Board_SPI_MASTER, &spiParams);
    if (masterSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing master SPI\n");
        while (1);
    }
    else {
        Display_printf(display, 0, 0, "Master SPI initialized\n");
    }

    /*
     * Master has opened Board_SPI_MASTER; set Board_SPI_MASTER_READY high to
     * inform the slave.
     */
    uint8_t setup_counter = 0;
    GPIO_write(Board_SPI_MASTER_READY, 0);

    while (1) {
        for (j = 0; j < PAYLOAD_LENGTH / 2; j++) {
            /* Copy message to transmit buffer */
            if (setup_counter <  num_messages) {
                masterTxBuffer[0] = messages[setup_counter];
            } else if (j < PAYLOAD_LENGTH / 2) { // data bits
                masterTxBuffer[0] = channels[j]; // change this
//                masterTxBuffer[0] = 0b0000000000000001;
            }

            GPIO_write(Board_GPIO_LED1, 0);

            /* Initialize master SPI transaction structure */
            memset((void *) masterRxBuffer, 0, SPI_MSG_LENGTH);
            transaction.count = SPI_MSG_LENGTH;
            transaction.txBuf = (void *) masterTxBuffer;
            transaction.rxBuf = (void *) masterRxBuffer;

            /* Perform SPI transfer */
            transferOK = SPI_transfer(masterSpi, &transaction);
            setup_counter++;
            if (setup_counter > 200) {
                setup_counter = 100;
            }

            GPIO_write(Board_GPIO_LED1, 1);
            packet[j*2] = (masterRxBuffer[0] >> 8);
            packet[j*2+1] = masterRxBuffer[0] & 0xff;
        }

        //Compression functions to modify packet info
//        if (setup_counter >  num_messages + 10){
//            store_packet_in_hist();
//            get_error_signal();
//            load_error_to_packet();
//        }


        /* Set absolute TX time to utilize automatic power management */
        curtime += PACKET_INTERVAL;
        RF_cmdPropTx.startTime = curtime;
        RF_EventMask terminationReason = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTx,
                                                           RF_PriorityNormal, NULL, 0);

        /* Sleep for a bit before starting the next SPI transfer  */
    }

    SPI_close(masterSpi);

    /* Example complete - set pins to a known state */
    GPIO_disableInt(Board_SPI_SLAVE_READY);
    GPIO_setConfig(Board_SPI_SLAVE_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_write(Board_SPI_MASTER_READY, 0);

    Display_printf(display, 0, 0, "\nDone");

    return (NULL);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Call driver init functions. */
    Display_init();
    GPIO_init();
    SPI_init();

    /* Configure the LED pins */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    // Enable the software chip select as output
    GPIO_setConfig(IOID_7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1);
    }

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Starting the SPI master example");
    Display_printf(display, 0, 0, "This example requires external wires to be "
        "connected to the header pins. Please see the Board.html for details.\n");

    /* Create application threads */
    pthread_attr_init(&attrs);


    //set up the registers
    int s = 0;
    // 0-9
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b1000000011011110; //R0
    messages[s++] = 0b1000000100100000; //R1
    messages[s++] = 0b1000001000101000; //R2
    messages[s++] = 0b1000001100000000; //R3
    messages[s++] = 0b1000010011001101; //R4
    messages[s++] = 0b1000010100000000; //R5
    messages[s++] = 0b1000011000000000; //R6
    messages[s++] = 0b1000011100000000; //R7
    messages[s++] = 0b1000100000001000; //R8
    messages[s++] = 0b1000100100000000; //R9
    messages[s++] = 0b1000101000000100; //R10
    messages[s++] = 0b1000101100000000; //R11
    messages[s++] = 0b1000110000010000; //R12
    messages[s++] = 0b1000110101111100; //R13
    messages[s++] = 0b1000111011111111; //R14
    messages[s++] = 0b1000111111111111; //R15
    messages[s++] = 0b1001000011111111; //R16
    messages[s++] = 0b1001000111111111; //R17
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0101010100000000; // Config
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b0000000000000000;
    messages[s++] = 0b1110100000000000; // I
    messages[s++] = 0b1110100100000000; // N
    messages[s++] = 0b1110101000000000; //
    messages[s++] = 0b1110101100000000;
    messages[s++] = 0b1110110000000000;
    messages[s++] = 0b1110100000000000; // I
    messages[s++] = 0b1110100100000000; // N
    messages[s++] = 0b1110101000000000; //
    messages[s++] = 0b1110101100000000;
    messages[s++] = 0b1110110000000000;
    messages[s++] = 0b1110100000000000; // I
    messages[s++] = 0b1110100100000000; // N
    messages[s++] = 0b1110101000000000; //
    messages[s++] = 0b1110101100000000;
    messages[s++] = 0b1110110000000000;


    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create master thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, masterThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    return (NULL);
}
