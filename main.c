#include <avr/io.h>
#include <util/delay.h>

void processButtonPress(int button);
void processButtonRelease(int button);

int pressed_confidence_level[2];
int released_confidence_level[2];
int pressed[2];
int ledNumber[2]; // 0 - 6, which LED needs to be lit next

int main(void) {
    // Initialization - Setting up ports and data direction registers
    DDRB = 0b01111111; // Set B data direction register, pin 8 as input
    DDRD = 0b01111111; // Set B data direction register, pin 8 as input
    PORTB |= 1 << PIN7; // Set Pin 7 as High
    PORTB |= 1 << PIN7; // Set Pin 7 as High

    while (1) {
        // Process button clicks for side 1
        if (bit_is_clear(PINB, 7)) {
            // Light up LED on Port B
            processButtonPress(0); 
        } else {
            // Side 1 button released
            processButtonRelease(0);
        }

        // Process button clicks for side 2
        if (bit_is_clear(PIND, 7)) {
            // Light up LED on Port D
            processButtonPress(1); 
        } else {
            // Side 2 button released
            processButtonRelease(1);
        }
    }
}


void processButtonPress(int button) {
    pressed_confidence_level[button]++;

    if (pressed_confidence_level[button] > 500) {
        if (pressed[button] == 0) {
            pressed[button] = 1;

            // Light up LED on Port B 
            if (button == 0) PORTB |= 1 << ledNumber[button];
            if (button == 1) PORTD |= 1 << ledNumber[button];
            ledNumber[button]++;

            // side 1 wins
            if (ledNumber[button] > 6) {
                // flash LEDs
                int i;
                for (i = 0; i < 10; i++) {
                    if (button == 0) PORTB = 0b11111111;
                    if (button == 1) PORTD = 0b11111111;
                    _delay_ms(100);
                    if (button == 0) PORTB = 0b10000000;
                    if (button == 1) PORTD = 0b10000000;
                    _delay_ms(100);
                }

                ledNumber[0] = 0;
                ledNumber[1] = 0;
                PORTB = 0b10000000;
                PORTD = 0b10000000;
            }
        }

        pressed_confidence_level[button] = 0;
    }
}

void processButtonRelease(int button) {
    released_confidence_level[button]++;
    if (released_confidence_level[button] > 500) {
        pressed[button] = 0;
        released_confidence_level[button] = 0;
    }
}
