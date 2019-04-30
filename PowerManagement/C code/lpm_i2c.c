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

void runI2C()
{
	//run I2C command, implement check and light up LED accordingly

    uint8_t upper_byte; //holds data from temperature sensor, change datatype as necessary for data from gauge
    uint8_t lower_byte;

    UCB2IFG &= ~(UCTXIFG + UCRXIFG0);       // Clear any pending interrupts

    UCB2CTLW0 |= UCTXSTT;                   //start condition

    while(!(UCB2IFG & UCTXIFG0));           // Wait for start to finish
    UCB2TXBUF = TEMP_REG;                   // Send temp register address

    while(!(UCB2IFG & UCTXIFG0));           // wait for address to finish sending

    UCB2CTLW0 &= ~UCTR;                     // Switch to receiver
    UCB2CTLW0 |= UCTXSTT;                   // Send start
    while(!(UCB2IFG & UCTXIFG0));

    while(!(UCB2IFG & UCRXIFG0));           // 1st byte
    upper_byte = UCB2RXBUF;

    UCB2CTLW0 |= UCTXSTP;      // Send stop condition

    while(!(UCB2IFG & UCRXIFG0));           // 2nd byte
    lower_byte = UCB2RXBUF;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //watchdog timer disable

    CSCTL0 = CSKEY; //set key to access clock registers
    CSCTL2 = SELA__VLOCLK; //set ACLK to source from VLO
    CSCTL3 = DIVA__32; //divide ACLK by 32 so we get long periods of LPM3 (wake every ~4 minutes)

    TA0CTL = TASSEL_1 | MC__UP | TACLR; //set Timer A to source ACLK, continuous mode, no divider, clear
    TA0CCTL0 = CCIE; //enable interrupts
    TA0CCR0 = 0xFFFF; //set timer threshold to highest possible value

    initGPIO();
    initI2C();

    __bis_SR_register(LPM3_bits | GIE); //enter LPM3 with interrupts

    while(1){} //wait forever so interrupts can do their thing
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    runI2C(); //get the temperature
    
    TA0CCR0 += 0xFFFF; //reset timer
}