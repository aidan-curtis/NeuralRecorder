#include <msp430.h>
#include <stdint.h>

#define GAUGE_ADDR 0x55

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
    UCB2I2CSA = GAUGE_ADDR;                   // Slave Address
    UCB2CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD; //disable watchdog timer
	initGPIO();
    initI2C();

	return 0;
}