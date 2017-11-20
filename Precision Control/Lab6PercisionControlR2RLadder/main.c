#include <msp430.h> 


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P3DIR |= 0xFF;
    P3OUT &= 0x00; // Set the LEDs off
    int dir = 1;
    while(1){
        if(P3OUT == 255){
            dir = 0;
        }
        if(P3OUT == 0){
            dir = 1;
        }
        if(dir == 1){
            P3OUT = P3OUT + 1;
        }else{
            P3OUT = P3OUT - 1;
        }

    }
}


