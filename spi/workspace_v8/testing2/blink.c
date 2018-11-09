//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  Texas Instruments, Inc
//  July 2013
//***************************************************************************************

#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT

    P1OUT |= BIT2; // Slave select set to 1
    P1DIR |= BIT2; // Enable output for slave select pin


    P1SEL = BIT0 | BIT1 | BIT2; // First part of putting these pins in spi mode
    P5SEL2 = BIT0 | BIT1 | BIT2; // First part of putting these pins in spi mode

    UCB0CTL1 = UCSWRST; // Set master mode to active
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2; // SMCLK
    UCB0BR0 |= 0x02; // /2
    UCB0BR1 = 0; //
    UCB0MCTL = 0; // No modulation
    UCB0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**


    P1OUT &= (~BIT5); // Select Device

    while (!(IFG2 & UCB0TXIFG)); // USCI_A0 TX buffer ready?
    UCB0TXBUF = 0xAA; // Send 0xAA over SPI to Slave
    while (!(IFG2 & UCB0RXIFG)); // USCI_A0 RX Received?
    received_ch = UCB0RXBUF; // Store received data

    P1OUT |= (BIT5); // Unselect Device
}
