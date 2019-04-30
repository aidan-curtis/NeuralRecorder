#include <msp430.h>

//put #defines here

void initGPIO()
{
    //set up pin 1 for LED operation
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    PM5CTL0 &= ~LOCKLPM5;
}

int main(void)
{
    //watchdog timer disable
    WDTCTL = WDTPW | WDTHOLD;

   CSCTL0 = CSKEY; //set key to access clock registers
   CSCTL2 = SELA__VLOCLK; //set ACLK to source from VLO
   CSCTL3 = DIVA__32; //divide ACLK by 32 so we get long periods of LPM3 (wake every ~4 minutes)

    initGPIO();
    //set Timer A to source ACLK, continuous mode, no divider, clear
    TA0CTL = TASSEL_1 | MC__UP | TACLR;
    TA0CCTL0 = CCIE; //enable interrupts
    TA0CCR0 = 0xFFFF; //set timer threshold to highest possible value

    __bis_SR_register(LPM3_bits | GIE); //enter LPM3 with interrupts

    while(1){} //wait forever so interrupts can do their thing
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    P1OUT ^= BIT0;
    TA0CCR0 += 0xFFFF;
}
