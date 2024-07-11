# Infrared Signal Driver

A program written in AVR C to receive Infrared signals transmitted with the
[NEC protocol](https://www.sbprojects.net/knowledge/ir/nec.php).

## [Receiver Test Program](src/main.c)

To test the functionality of receiving IR signals, this program uses 3 LEDs,
each of which can be toggled on and off using a different IR command (or different
button on an IR remote). Repeating signals can also be tested by pressing and holding
a button on an IR remote to see LED(s) be repeatedly toggled on and off.

## [IR Driver](src/ir_driver.h)

This program sets up all necessary interrupts and will automatically begin decoding
received data when an IR signal is detected. Currently only supports receiving signals
transmitted with the NEC protocol.

#### `IRData`

The datatype that contains all data from the IR receiver. The address, command, and raw data
from the receiver are all stored in this datatype.

#### `irInit()`

A method that initializes all necessary interrupts to detect and decode IR signals 

#### `irDisable()`

A method that resets all necessary interrupts to detect and decode IR signals

#### `irReceive()`

A method that checks if any data has been received by an IR receiver since the
last time the method was called.
