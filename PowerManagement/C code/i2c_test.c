#include <msp430.h>
#include <stdint.h>

#define SLAVE_ADDR  0x18
#define TEMP_REG 0x05

void initGPIO()
{
    // I2C pins
    P7SEL0 |= BIT0 | BIT1;
    P7SEL1 &= ~(BIT0 | BIT1);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}


void initI2C()
{
    UCB2CTLW0 = UCSWRST;                      // Enable SW reset
    UCB2CTLW0 |= 0x0600 | UCMST | UCSSEL_3 | UCTR_1; // I2C master mode, SMCLK, transmitter
    UCB2BRW = 10;                            // fSCL = SMCLK/10 = ~100kHz
    UCB2I2CSA = SLAVE_ADDR;                   // Slave Address
    UCB2CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
}

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    initGPIO();
    initI2C();
    uint8_t upper_byte;
    uint8_t lower_byte;
    float temp;

    UCB2IFG &= ~(UCTXIFG + UCRXIFG0);       // Clear any pending interrupts

    UCB2CTLW0 |= UCTXSTT; //start condition

    while(!(UCB2IFG & UCTXIFG0));           // Wait for start to finish
    UCB2TXBUF = TEMP_REG;                   // Send temp register address

    while(!(UCB2IFG & UCTXIFG0));

    UCB2CTLW0 &= ~UCTR;                     // Switch to receiver
    UCB2CTLW0 |= UCTXSTT;                   // Send start
    while(!(UCB2IFG & UCTXIFG0));

    while(!(UCB2IFG & UCRXIFG0));           // 1st byte
    upper_byte = UCB2RXBUF;

    UCB2CTLW0 |= UCTXSTP;      // Send stop condition

    while(!(UCB2IFG & UCRXIFG0));           // 2nd byte
    lower_byte = UCB2RXBUF;



    return 0;
}

