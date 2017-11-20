
#include <msp430.h>

 // Initial Greeting you should see upon properly connecting your Launchpad
int i = 0;
volatile int count = 0;
volatile int BitRX = 0;
int main(void)
{

    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    //Initialize LEDs for pwm
    P1DIR |= 0x1C;  //LEDs set as output
    P1SEL |= 0x1C;  //LEDs set to TA0.1/2/3

    //Set up CCRs for clock count and pwm control
    TA0CCR0  = 100;      //After timerA counts to 255 reset
    TA0CCTL1 = OUTMOD_7;    //Put Capture Control 1 in set and reset mode
    TA0CCR1 = 25;            //Initialize Capture Control Register 1 to 0
    TA0CTL = TASSEL_2 + MC_1 + ID_3;    //TimerA uses SMCLK, with divider of 8, in count up mode
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
    __no_operation();                         // For debugger


}



