/*
 * Author: Sean McGuire
 * Last Edited: 4:43
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
#include "msp430.h"
#include "math.h"
#include "LCDDriver.h"
float value = 0;

void uart_init();
void PWM_init();
void LCD_init();
void GPIOConfig();
void ADC_init();
void tempToPWM(int temp);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    LCD_init();
    GPIOConfig();
    uart_init();
    PWM_init();
    ADC_init();
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT


    // Configure Timer0_A
    TA0CCTL0 = CCIE;   // CCR0 interrupt enabled
    TA0CCR0 = 0x3FFF;                          // PWM Period
    TA0CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
    __enable_interrupt(); // Enables all interrupts
    while (1)
    {

            ADC12CTL0 |= ADC12ENC | ADC12SC;    // Start sampling/conversion
            __bis_SR_register(LPM0_bits | GIE); // LPM0, ADC12_ISR will force exit
            __no_operation();                   // For debugger
    }
}


void uart_init()
{
    // Configure GPIO
    P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 UART operation
    P2SEL1 &= ~(BIT0 | BIT1);

    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void GPIOConfig()
{
    P1DIR = 0xFF; P2DIR = 0xFF;
    P1REN = 0xFF; P2REN = 0xFF;
    P1OUT = 0x00; P2OUT = 0x00;
}
void LCD_init(){

        PJSEL0 = BIT4 | BIT5;                   // For LFXT
        // Initialize LCD_C
        PM5CTL0 &= ~LOCKLPM5;

        // Initialize LCD segments 0 - 21; 26 - 43
           LCDCPCTL0 = 0xFFFF;
           LCDCPCTL1 = 0xFC3F;
           LCDCPCTL2 = 0x0FFF;
        // Configure LFXT 32kHz crystal
            CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
            CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
            do
            {
              CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
              SFRIFG1 &= ~OFIFG;
            }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
            CSCTL0_H = 0;                           // Lock CS registers

        // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
        LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

        // VLCD generated internally,
        // V2-V4 generated internally, v5 to ground
        // Set VLCD voltage to 2.60v
        // Enable charge pump and select internal reference for it
        LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

        LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

        LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
        //Turn LCD on
        LCDCCTL0 |= LCDON;

}
void PWM_init()
{
    // Configure GPIO
    P3DIR |= BIT6;
    P3SEL1 |= BIT6;

    // Configure Timer0_B
    TB0CCR0 = 255;                          // PWM Period
    TB0CCTL2 = OUTMOD_7;                      // CCR1 reset/set
    TB0CCR2 = 0;                             // CCR1 PWM duty cycle
    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;   // ACLK, up mode, clear TAR
}

void ADC_init(){
    P9SEL1 |= BIT4;                           // Configure P1.1 for ADC
    P9SEL0 |= BIT4;

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_12;              // A1 ADC input select; Vref=AVCC
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt

}

void tempToPWM(int temp)
{
    int pwm;
    float tp;
    tp = temp/100.0;						//convert voltage in mV to temp reading
    if(tp < 0.41){
		pwm = -3138.9*tp + 1377.8;			// set PWM value based upon linear function if less than 41 degrees celcius is the desired temp
    }else{
        pwm = -220.56*tp + 163.16;			// set PWM value based upon linear function if greater than 41 degrees celcius is the desired temp
    }
    if(pwm < 0){		
        pwm = 0;							// Set PMW to 0 if value is supposed to be negative
    }
    if(pwm > 0 & pwm < 31){					// If PWM value is expected to be between 0 and 31, set output on full and wait 1 second before returning to deswignated value to kick start system
        TB0CCR2 = 255;
        __delay_cycles(1000000);
    }
    TB0CCR2 = pwm;
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
    {
        case ADC12IV_NONE:        break;    // Vector  0:  No interrupt
        case ADC12IV_ADC12OVIFG:  break;    // Vector  2:  ADC12MEMx Overflow
        case ADC12IV_ADC12TOVIFG: break;    // Vector  4:  Conversion time overflow
        case ADC12IV_ADC12HIIFG:  break;    // Vector  6:  ADC12BHI
        case ADC12IV_ADC12LOIFG:  break;    // Vector  8:  ADC12BLO
        case ADC12IV_ADC12INIFG:  break;    // Vector 10:  ADC12BIN
        case ADC12IV_ADC12IFG0:             // Vector 12:  ADC12MEM0 Interrupt

                value = ADC12MEM0;
                //Convert to voltage
                unsigned int j;
                unsigned int i;
                unsigned int tmp = 0;
                float t;
                for(j = 0; j < 4; j ++){		//Cycling through temp digits
                
                    t = value*pow(10,j);		//seperating adc value by digit

                    if(tmp == 1){
                        showChar('1',j+1);
                        break;
                    }else if(tmp == 2){
                        showChar('2',j+1);
                        break;
                    }else if(tmp == 3){
                        showChar('3',j+1);
                        break;
                    }else if(tmp == 4){
                        showChar('4',j+1);
                        break;
                    }else if(tmp == 5){
                        showChar('5',j+1);
                        break;
                    }else if(tmp == 6){
                        showChar('6',j+1);
                        break;
                    }else if(tmp == 7){
                        showChar('7',j+1);
                        break;
                    }else if(tmp == 8){
                        showChar('8',j+1);
                        break;
                    }else if(tmp == 9){
                        showChar('9',j+1);
                        break;
                    }else{
                        showChar('0',j+1);
                        break;
                    }
                }
                // Exit from LPM0 and continue executing main
                __bic_SR_register_on_exit(LPM0_bits);
            break;
        case ADC12IV_ADC12IFG1:   break;    // Vector 14:  ADC12MEM1
        case ADC12IV_ADC12IFG2:   break;    // Vector 16:  ADC12MEM2
        case ADC12IV_ADC12IFG3:   break;    // Vector 18:  ADC12MEM3
        case ADC12IV_ADC12IFG4:   break;    // Vector 20:  ADC12MEM4
        case ADC12IV_ADC12IFG5:   break;    // Vector 22:  ADC12MEM5
        case ADC12IV_ADC12IFG6:   break;    // Vector 24:  ADC12MEM6
        case ADC12IV_ADC12IFG7:   break;    // Vector 26:  ADC12MEM7
        case ADC12IV_ADC12IFG8:   break;    // Vector 28:  ADC12MEM8
        case ADC12IV_ADC12IFG9:   break;    // Vector 30:  ADC12MEM9
        case ADC12IV_ADC12IFG10:  break;    // Vector 32:  ADC12MEM10
        case ADC12IV_ADC12IFG11:  break;    // Vector 34:  ADC12MEM11
        case ADC12IV_ADC12IFG12:  break;    // Vector 36:  ADC12MEM12
        case ADC12IV_ADC12IFG13:  break;    // Vector 38:  ADC12MEM13
        case ADC12IV_ADC12IFG14:  break;    // Vector 40:  ADC12MEM14
        case ADC12IV_ADC12IFG15:  break;    // Vector 42:  ADC12MEM15
        case ADC12IV_ADC12IFG16:  break;    // Vector 44:  ADC12MEM16
        case ADC12IV_ADC12IFG17:  break;    // Vector 46:  ADC12MEM17
        case ADC12IV_ADC12IFG18:  break;    // Vector 48:  ADC12MEM18
        case ADC12IV_ADC12IFG19:  break;    // Vector 50:  ADC12MEM19
        case ADC12IV_ADC12IFG20:  break;    // Vector 52:  ADC12MEM20
        case ADC12IV_ADC12IFG21:  break;    // Vector 54:  ADC12MEM21
        case ADC12IV_ADC12IFG22:  break;    // Vector 56:  ADC12MEM22
        case ADC12IV_ADC12IFG23:  break;    // Vector 58:  ADC12MEM23
        case ADC12IV_ADC12IFG24:  break;    // Vector 60:  ADC12MEM24
        case ADC12IV_ADC12IFG25:  break;    // Vector 62:  ADC12MEM25
        case ADC12IV_ADC12IFG26:  break;    // Vector 64:  ADC12MEM26
        case ADC12IV_ADC12IFG27:  break;    // Vector 66:  ADC12MEM27
        case ADC12IV_ADC12IFG28:  break;    // Vector 68:  ADC12MEM28
        case ADC12IV_ADC12IFG29:  break;    // Vector 70:  ADC12MEM29
        case ADC12IV_ADC12IFG30:  break;    // Vector 72:  ADC12MEM30
        case ADC12IV_ADC12IFG31:  break;    // Vector 74:  ADC12MEM31
        case ADC12IV_ADC12RDYIFG: break;    // Vector 76:  ADC12RDY
        default: break;
    }
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      int i = UCA0RXBUF;
      UCA0TXBUF = UCA0RXBUF;
      tempToPWM(i);


      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    value = ADC12MEM0;
    //Convert to voltage
    value = value*.00080566;
    unsigned int trsmt = value * 10000;
    unsigned int t1 = trsmt>>8;
    UCA0TXBUF = t1;
    UCA0TXBUF = trsmt;

}
