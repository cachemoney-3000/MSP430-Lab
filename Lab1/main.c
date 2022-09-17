#include <msp430fr6989.h>
#include <stdint.h>
#define redLED BIT0
#define greenLED BIT7

/**
 * main.c
 */

void blinkRedLED();
void blinkRedAndGreenLEDInSync();
void blinkRedAndGreenLEDOutSync();
void longerDelay();
void longerDelay2();
void longerDelay3();
void fireTruckLEDPattern1();
void fireTruckLEDPattern2();

void main(void) {

    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default highimpedance mode

    //blinkRedLED();

    //blinkRedAndGreenLEDInSync();
    //blinkRedAndGreenLEDOutSync();

    //longerDelay();
    //longerDelay2();
    //longerDelay3();

    //fireTruckLEDPattern1();
    //fireTruckLEDPattern2();

    volatile unsigned int i;
       // RED LED
       P1DIR |= redLED; // Direct pin as output
       P1OUT &= ~redLED; // Turn LED Off

       // GREEN LED
       P9DIR |= greenLED;
       P9OUT &= ~greenLED;

       P1OUT &= ~redLED;
       for(;;) {
           int j;
           int delay = 20000;
           // Blink the two LEDs 20 times alternately
           for (j = 0; j < 20; j++) {
               for(i=0; i<delay; i++) {}
               P1OUT ^= redLED;    // Flash the Red LED

               P9OUT &= ~greenLED; // Turn Green LED Off
               for(i=0; i<delay; i++) {}
               P9OUT ^= greenLED;  // Flash the green LED

               // Decrement the delay
               if (i > 6)
                   delay = delay - 1000;

               P1OUT &= ~redLED; // Turn Red LED Off
           }

           // Turn off both LEDs
           P1OUT &= ~redLED;
           P9OUT &= ~greenLED;

           // BLINK TWO LEDS
           int k;
           for (k =0; k < 15; k++){
               // Delay loop
               for(i=0; i<20000; i++) {}
               P1OUT ^= redLED;    // Toggle the Red LED
               P9OUT ^= greenLED;  // Toggle the green LED
           }

           // Turn off both LEDs
           P1OUT &= ~redLED;
           P9OUT &= ~greenLED;
       }

}

void blinkRedLED() {
    volatile unsigned int i;
    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off

    for(;;) {
        // Delay loop
        for(i=0; i<20000; i++) {}
        P1OUT ^= redLED; // Toggle the LED
    }
}

void longerDelay() {
    volatile unsigned int i;

    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off


    for(;;) {
        // Delay loop
        for(i=0; i<65535; i++) {}
        P1OUT ^= redLED; // Toggle the LED
    }
}

void longerDelay2() {
    volatile uint32_t i;

    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off


    for(;;) {
        // Delay loop
        for(i=0; i<80000; i++) {}
        P1OUT ^= redLED; // Toggle the LED
    }
}

void longerDelay3() {
    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off


    for(;;) {
        // Delay loop 900000 CPU cycles
        _delay_cycles(900000);
        P1OUT ^= redLED; // Toggle the LED
    }
}

void blinkRedAndGreenLEDInSync() {
volatile unsigned int i;
    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off

    // GREEN LED
    P9DIR |= greenLED;
    P9OUT &= ~greenLED;


    for(;;) {
        // Delay loop
        for(i=0; i<20000; i++) {}
        P1OUT ^= redLED;    // Toggle the Red LED
        P9OUT ^= greenLED;  // Toggle the green LED
    }
}

void blinkRedAndGreenLEDOutSync() {
    volatile unsigned int i;
    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off

    // GREEN LED
    P9DIR |= greenLED;
    P9OUT &= ~greenLED;


    for(;;) {
        // Delay loop for the redLED
        for(i=0; i<10000; i++) {}
        P1OUT ^= redLED;    // Toggle the Red LED

        // Delay loop for the greenLED
        for(i=0; i<10000; i++) {}
        P9OUT ^= greenLED;  // Toggle the green LED
    }
}

void fireTruckLEDPattern1() {
    volatile unsigned int i;
        // RED LED
        P1DIR |= redLED; // Direct pin as output
        P1OUT &= ~redLED; // Turn LED Off

        // GREEN LED
        P9DIR |= greenLED;
        P9OUT &= ~greenLED;


        for(;;) {
            // Delay loop for the redLED
            int j;
            for(j = 0; j < 10; j++){
                for(i=0; i<5000; i++) {}
                P1OUT ^= redLED;    // Toggle the Red LED
            }

            P1OUT &= ~redLED;

            int k;
            for (k = 0; k < 10; k++) {
                // Delay loop for the greenLED
                for(i=0; i<5000; i++) {}
                P9OUT ^= greenLED;  // Toggle the green LED
            }

            P9OUT &= ~greenLED;

        }
}

void fireTruckLEDPattern2() {
    volatile unsigned int i;
    // RED LED
    P1DIR |= redLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off

    // GREEN LED
    P9DIR |= greenLED;
    P9OUT &= ~greenLED;

    P1OUT &= ~redLED;
    for(;;) {
        int j;
        int delay = 20000;
        // Blink the two LEDs 20 times alternately
        for (j = 0; j < 20; j++) {
            for(i=0; i<delay; i++) {}
            P1OUT ^= redLED;    // Flash the Red LED

            P9OUT &= ~greenLED; // Turn Green LED Off
            for(i=0; i<delay; i++) {}
            P9OUT ^= greenLED;  // Flash the green LED

            // Decrement the delay
            if (i > 6)
                delay = delay - 1000;

            P1OUT &= ~redLED; // Turn Red LED Off
        }

        // Turn off both LEDs
        P1OUT &= ~redLED;
        P9OUT &= ~greenLED;

        // BLINK TWO LEDS
        int k;
        for (k =0; k < 15; k++){
            // Delay loop
            for(i=0; i<20000; i++) {}
            P1OUT ^= redLED;    // Toggle the Red LED
            P9OUT ^= greenLED;  // Toggle the green LED
        }

        // Turn off both LEDs
        P1OUT &= ~redLED;
        P9OUT &= ~greenLED;
    }
}
