#include "ir_driver.h"
#include <avr/interrupt.h>
#include <stdbool.h>

#define MS_TICKS 78   // Number of ticks that constitute ~1ms. Defaults to prescalar of 256 and CPU frequency of 20MHz
#define TIMEOUT  120  // Timeout to stop listening for IR signals (in ms)

volatile unsigned char timer, pulseCount;
volatile unsigned long int rawData;
volatile bool messageReceived;

#define ADDRESS  ((rawData & 0xFF000000) >> 24)
#define INV_ADDR ((rawData & 0x00FF0000) >> 16)
#define COMMAND  ((rawData & 0x0000FF00) >> 8)
#define INV_CMD   (rawData & 0x000000FF)

void reset() {
    timer = 0;
    pulseCount = 0;
    rawData = 0;
}

void startTimer() {
    TCNT0  =  0;
    TCCR0B = (1 << CS02);
    timer  =  0;
}

void stopTimer() {
    TCCR0B = 0;
}

unsigned char reverse(unsigned char data) {
    unsigned char reversed = 0;
    unsigned char temp;
    for (int i = 0; i < 8; i++) {
        temp = data;
        temp <<= 7 - i;
        temp >>= 7;
        reversed <<= 1;
        reversed += temp;
    }
    return reversed;
}

void irInit() {
    // Setup interrupt every 1ms
    TCCR0A |= (1 << WGM01);
    OCR0A   =  MS_TICKS;
    TIMSK0 |= (1 << OCIE0A);

    // Setup interrupt every falling edge from receiver
    EICRA  |= (1 << ISC01);
    EIMSK  |= (1 << INT0);

    sei();
}

void irDisable() {
    TIMSK0 &= ~(1 << OCIE0A);
    EIMSK  &= ~(1 << INT0);
}

IRData irReceive() {
    IRData receivedData = {0, 0, 0};
    if (messageReceived) {
        receivedData.address = reverse(ADDRESS);
        receivedData.command = reverse(COMMAND);
        receivedData.rawData = rawData;
        messageReceived = false;
    }
    return receivedData;
}

ISR(TIMER0_COMPA_vect) {
    timer++;
    if (timer > TIMEOUT) {
        reset();
        stopTimer();
    }
}

ISR(INT0_vect) {
    stopTimer();
    pulseCount++;

    if (3 <= pulseCount && pulseCount <= 34) {
        rawData <<= 1;
        rawData  += (timer > 0) ? 1 : 0;
    }

    if (pulseCount >= 34 && !(pulseCount & 1)) {
        unsigned char inv_addr = ~INV_ADDR;
        unsigned char inv_cmd = ~INV_CMD;
        messageReceived = (ADDRESS == inv_addr) && (COMMAND == inv_cmd);
    }
    
    startTimer();
}
