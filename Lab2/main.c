#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at P1.1
#define BUT2 BIT2 // Button S1 at P1.2

void main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

    // Configure and initialize LEDs
    P1DIR |= redLED; // Direct pin as output
    P9DIR |= greenLED; // Direct pin as output

    P1OUT &= ~redLED; // Turn LED Off
    P9OUT &= ~greenLED; // Turn LED Off

    // Configure buttons
        P1DIR &= ~(BUT1 | BUT2); // Direct pin as input
        P1REN |= (BUT1 | BUT2); // Enable built-in resistor
        P1OUT |= (BUT1 | BUT2); // Set resistor as pull-up
        int released = 1;

        // Only one LED will be able to turn on
        for(;;) {
        P9OUT &= ~greenLED; // Turn green LED off
        P1OUT &= ~redLED; // Turn red LED off

        if((P1IN & (BUT2|BUT1)) == 0){
            released = 0;
        }

        if((P1IN & (BUT2|BUT1)) == (BUT2|BUT1)){
            released = 1;
        }

        // Check if button 1 is zero then turn on red LED
        if ((P1IN & (BUT2|BUT1)) == BUT2 && released == 1){
            P1OUT |= redLED; // Turn red LED on
            P9OUT &= ~greenLED; // Turn green LED off

        }

        // Check if button 2 is zero then turn on green LED
        if ((P1IN & (BUT2|BUT1)) == BUT1 && released == 1){
            P9OUT |= greenLED; //Turn green LED on
            P1OUT &= ~redLED; // Turn red LED off
        }
    }
}
