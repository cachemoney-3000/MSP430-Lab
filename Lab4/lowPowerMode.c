// Timer_A continuous mode, with interrupt, flashes LEDs
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at Port 1.1
#define BUT2 BIT2 // Button S2 at Port 1.2

void config_ACLK_to_32KHz_crystal();
void timerContinousMode();
void timerUpMode();
void pushButtonWithInterrupt();

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Enable the GPIO pins

    P1DIR |= redLED;            // Direct pin as output
    P9DIR |= greenLED;          // Direct pin as output
    P1OUT &= ~redLED;           // Turn LED Off
    P9OUT &= ~greenLED;         // Turn LED Off

    P1DIR &= ~ (BUT1|BUT2);     // 0: input
    P1REN |= (BUT1|BUT2);       // 1: enable built-in resistors

    P1OUT |= (BUT1|BUT2);       // 1: built-in resistor is pulled up to Vcc
    P1IES |= (BUT1|BUT2);       // 1: interrupt on falling edge
    P1IFG &= ~ (BUT1|BUT2);     // 0: clear the interrupt flags
    P1IE |= (BUT1|BUT2);        // 1: enable interrupts

    // Configure ACLK to the 32 KHz crystal
    config_ACLK_to_32KHz_crystal();

    timerContinousMode();
    //timerUpMode();
    //pushButtonWithInterrupt();
}

//------------------------------------------------------------------//

void pushButtonWithInterrupt() {
    // Configure Channel 0 for up mode with interrupt
    TA0CCR0 = 16000-1; // Fill to get 1 second @ 32 KHz
    TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
    TA0CCTL0 &= ~ (CCIFG); // Clear Channel 0 CCIFG bit

    _low_power_mode_3();
}

#pragma vector = PORT1_VECTOR // link the ISR to the vector
__interrupt void Port1_ISR() {
    //if button 1 is pressed
    if ((P1IFG & BUT1) != 0) {
        P1OUT ^= redLED;    //toggle the red LED
        P1IFG &= ~BUT1;     //clear button 1
    }
    //if button 2 is pressed
    if ((P1IFG & BUT2) != 0) {
        P9OUT ^= greenLED;  //toggle the green LED
    P1IFG &= ~BUT2;         // clear button 2
    }
}

//------------------------------------------------------------------//
void timerUpMode(){
    // Configure Channel 0 for up mode with interrupt
    TA0CCR0 = 32768 - 1;        // 1 second @ 32 KHz
    TA0CCTL0 |= CCIE;           // Enable Channel 0 CCIE bit
    TA0CCTL0 &= ~ (CCIFG);      // Clear Channel 0 CCIFG bit

    // Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    _low_power_mode_3();
}

//******* Writing the ISR *******
#pragma vector = TIMER0_A0_VECTOR   // Link the ISR to the vector
__interrupt void T0A0_ISR() {
    P1OUT ^= redLED;                // Toggle both LEDs when interrupt is raised
    P9OUT ^= greenLED;
    //TA0CTL &= ~TAIFG;               // Clear the TAIFG flag
}

//------------------------------------------------------------------//
void timerContinousMode(){
    // Timer_A configuration
    // Use ACLK, divide by 1, continuous mode, TAR cleared, enable interrupt for rollback-to-zero event
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR | TAIE;
    // Ensure the flag is cleared at the start
    TA0CTL &= ~TAIFG;

    _low_power_mode_3();
}

//******* Writing the ISR *******
#pragma vector = TIMER0_A1_VECTOR   // Link the ISR to the vector
__interrupt void T0A1_ISR() {
    P1OUT ^= redLED;                // Toggle both LEDs when interrupt is raised
    P9OUT ^= greenLED;
    TA0CTL &= ~TAIFG;               // Clear the TAIFG flag
}

//------------------------------------------------------------------//
// Configures ACLK to 32 KHz crystal
void config_ACLK_to_32KHz_crystal() {
    // By default, ACLK runs on LFMODCLK at 5MHz/128 = 39 KHz
    // Reroute pins to LFXIN/LFXOUT functionality
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    // Wait until the oscillator fault flags remain cleared
    CSCTL0 = CSKEY; // Unlock CS registers

    do {
        CSCTL5 &= ~LFXTOFFG; // Local fault flag
        SFRIFG1 &= ~OFIFG; // Global fault flag
    } while((CSCTL5 & LFXTOFFG) != 0);

    CSCTL0_H = 0; // Lock CS registers

    return;
}
