/**
 * Driver to receive infrared signals transmitted with the NEC protocol on an ATmega328P (Arduino UNO). Also supports
 * repeated signals (ex. if a button on an IR remote is pressed and held).
 * 
 * An IR receiver must be connected to pin 4 of the ATmega328P (pin 2 of the Arduino UNO).
 */

#ifndef IR_DRIVER_H
#define IR_DRIVER_H

/**
 * Datatype to hold data pertaining to a received IR signal.
 * If no message is received or an invalid message is received,
 * all fields of this datatype will default to 0.
 */
typedef struct {
    unsigned char address, command;
    unsigned long int rawData;
} IRData;

/**
 * Initialize interrupts to begin listening for IR signals.
 */
void irInit();

/**
 * Disable interrupts to stop receiving IR signals.
 */
void irDisable();

/**
 * Retrieve any IR data that has been received from the sensor
 * 
 * @return Data received from IR receiver
 */
IRData irReceive();

#endif