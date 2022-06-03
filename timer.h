#ifndef TIMER_H
#define TIMER_H

void timer_setup(int baud_rate) {
    int frequency = constrain(baud_rate, 1, 1500);
    // Set timer1 interrupt
    TCCR1A = 0; // Set entire TCCR1A register to 0
    TCCR1B = 0; // Same for TCCR1B
    TCNT1  = 0; // Initialize counter value to 0
    // OCR1A = counter; // = (16 * 10^6) / (10 * 1024) - 1 (must be < 65536)
    OCR1A = ((16 * pow(10, 6)) / (1024 * frequency)) - 1;
    // Turn on CTC mode (clear time on compare)
    TCCR1B |= (1 << WGM12);
    // Turn T1 off
    TCCR1B &= 0xF8;
    // Disable interrupts
    TIMSK1 = 0;
    TIFR1 = 0;
}

void timer_start() {
    Serial.println("Timer started");
    TCNT1 = 0; // Initialize counter value to 0
    TIFR1 = 0;
    // Enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void timer_stop() {
    Serial.println("Timer stopped");
    // Turn T1 off
    TCCR1B &= 0xF8;
    TIMSK1 = 0;
}

#endif
