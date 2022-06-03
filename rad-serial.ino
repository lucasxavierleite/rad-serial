#define PIN_RX 13
#define PIN_TX 13
#define BAUD_RATE 1
#define HALF_BAUD 1000 / (2 * BAUD_RATE)

#include "timer.h"

// Calculates parity bit - even or odd
bool parity_bit(char data) {

}

// timer1 interrupt routine
// Runs every 1 second
ISR(TIMER1_COMPA_vect) {
    // >>>> Code here <<<<
}

void setup() {
    // Disable interrupts
    noInterrupts();
    // Setup serial port (Serial Monitor - Ctrl + Shift + M)
    Serial.begin(9600);
    // Initialize TX or RX
    // >>>> Code here <<<<
    // Setup timer
    // >>>> Code here <<<<
    // Enable interrupts
    interrupts();
}

void loop() {
    // >>>> Code here <<<<
}
