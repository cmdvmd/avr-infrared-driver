/**
 * Program to test receiving infrared signals on the ATmega328P over the NEC protocol
 * 
 * Connect IR Receiver to pin 4 on the ATmega328P (pin 2 on Arduino UNO)
 * and 3 LEDs to pins 14-16 on the ATmega328P (pins 8-10 on Arduino UNO)
 * 
 * Each LED is toggled using a specific command/button (which can be changed below)
 * from an IR remote that transmits over the NEC protocol. Additionally, one command/button
 * corresponds to a power button, which will force all LEDs to be on/off. If any LED(s) are
 * on, all LEDs will be forced off. If all LEDs are turned off, they will all be forced on.
 */

#define F_CPU 20000000

#define PB0_LED_COMMAND 0x0C
#define PB1_LED_COMMAND 0x18
#define PB2_LED_COMMAND 0x5E
#define PWR_BTN_COMMAND 0x45

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ir_driver.h"


int main() {
    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2);

    irInit();

    IRData data;
    while (1) {
        data = irReceive();
        if (data.command == PB0_LED_COMMAND) {
            PORTB ^= (1 << PB0);
        }
        else if (data.command == PB1_LED_COMMAND) {
            PORTB ^= (1 << PB1);
        }
        else if (data.command == PB2_LED_COMMAND) {
            PORTB ^= (1 << PB2);
        }
        else if (data.command == PWR_BTN_COMMAND) {
            if ((PINB & PB0) || (PINB & PB1) || (PINB & PB2)) {
                PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
            }
            else {
                PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
            }
        }
        _delay_ms(100);
    }
}
