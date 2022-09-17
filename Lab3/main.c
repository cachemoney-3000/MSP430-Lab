#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at p1.1
#define BUT2 BIT2 // Button S2 at P1.2

void config_ACLK_to_32KHz_crystal();
void continousMode();
void upMode();
void signalRepeater();

int main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins
    // Configure and initialize LEDs
    P1DIR |= redLED; // Direct pin as output
    P9DIR |= greenLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off
    P9OUT &= ~greenLED; // Turn LED Off

    // Configure Buttons
    P1DIR &= ~(BUT1|BUT2); // Direct pin as input
    P1REN |= (BUT1|BUT2); //Enable in resistor
    P1OUT |= (BUT1|BUT2); // pull UP
    // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();

    //continousMode();
    //upMode();
    signalRepeater();
}


//**********************************
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

void continousMode(){
    // Configure Timer_A
    // Use ACLK, divide by 1, continuous mode, clear TAR
    TA0CTL = TASSEL_1 | ID_1 | MC_2 | TACLR;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    // Infinite loop
    for(;;) {
        // Wait in this empty loop for the flag to raise
        // About 2 secs delay (65535 cycles / 32768 Hz = 2 s)
        while ((TA0CTL & TAIFG) == 0) {}

        P1OUT ^= redLED; //Toggle the red LED
        TA0CTL &= ~TAIFG; // Clear the flag
    }
}

void upMode(){
    // Set timer period
    // Time in seconds = cycles / 32KHz
    TA0CCR0 = 32768 - 1;

    //TA0CCR0 = 3277 - 1;   // 0.1 s = 3277 / 32KHz
    //TA0CCR0 = 328 - 1;    // 0.01 s = 328 / 32KHz

    // Configure Timer_A
    // Use ACLK, divide by 1, up mode, clear TAR
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    // Infinite loop
    for(;;) {
        // Wait in this empty loop for the flag to raise
        // About 2 secs delay (65535 / 32768 Hz = 2 s)
        while ((TA0CTL & TAIFG) == 0) {}

        P1OUT ^= redLED; //Toggle the red LED
        TA0CTL &= ~TAIFG; // Clear the flag
    }
}

void signalRepeater(){
    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    int longDelay = 0;
    int buttonPressed = 0;
    // Infinite loop for polling
    for(;;){
        // Check if the longDelay flag is raised
        if (longDelay == 1){
            // Check if the Button S2 was pressed then reset the longDelay flag
            if((P1IN & BUT2) == 0) longDelay = 0;

            P9OUT ^= greenLED;   // Turn on the green LED
        }

        // The longDelay flag is not raised
        else{
            P9OUT &= ~greenLED; // Turn off the green LED
            // Check if S1 button is pressed
            if ((P1IN & BUT1) == 0){
                TA0CTL = TASSEL_1 | ID_0 |  MC_2 | TACLR;   // Start the timer

                // Empty loop that will keep looping as long as the button is pressed
                while ((P1IN & BUT1) == 0){}

                // Check if the timer exceeds the max cycles
                if ((TA0CTL & TAIFG) == TAIFG) longDelay = 1;

                buttonPressed = 1;    // Set to 1 (button was pressed)
            }

            TA0CTL |= MC_0;     // Stop the timer
            TA0CTL &= ~TAIFG;   // Clear the flag

            // Check if buttonPressed flag is raised and longDelay flag is not raised
            if (buttonPressed == 1 && longDelay == 0){
                TA0CTL |= MC_1;     // Start the Up timer

                // Toggle the red LED and stop when the flag is raised
                P1OUT ^= redLED;
                while ((TA0CTL & TAIFG) == 0) {}
                P1OUT &= ~redLED;   // Turn off the red LED
            }
            buttonPressed = 0;  // Reset the buttonPressed flag
        }

    }
}
