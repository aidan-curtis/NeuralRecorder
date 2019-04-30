#include <msp430.h>

//put #defines here

void initGPIO()
{
    //set up pin 1 for LED operation
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    PM5CTL0 &= ~LOCKLPM5;
}

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // Stop WDT
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode
  TA0CCR0 =  10000;                     // 12.5 Hz
  initGPIO();
  __bis_SR_register(LPM3_bits + GIE);          // Enter LPM3 w/ interrupt
  while(1)                          //Loop forever, we work with interrupts!
  {}
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
   P1OUT ^= BIT0;                          // Toggle P1.0
}
