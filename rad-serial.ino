#include "timer.h"

#define PIN_RX 13
#define PIN_RTS 12
#define PIN_CTS 11

#define BAUD_RATE 1
#define HALF_BAUD 1000 / (2 * BAUD_RATE)

#define BYTE_SIZE 8

bool check_parity = false;
char current_position = 0; // Bit position inside a char
char current_char = 0;

char message[256];
unsigned int message_count = 0;

// Calculates parity bit (checks if it's even)
bool parity_bit(char c) {
    unsigned char count = 0;

    for (unsigned char i = 0; i < BYTE_SIZE; i++)
        count += bitRead(c, i);

    return count % 2 == 0;
}

// timer1 interrupt routine
// Runs every 1 second
ISR(TIMER1_COMPA_vect) {
    bool received_bit = digitalRead(PIN_RX);

    char output[40];

    if (current_position == 0) { // Start bit
        if(received_bit == 0) return;
        Serial.print("SB: ");
    } else if (current_position == 9) { // Parity bit
        check_parity = (parity_bit(current_char) == received_bit);
        Serial.print("PB: ");
    } else if (current_position == 10) { // End bit
        sprintf(output, "EB: %u", received_bit);
        Serial.println(output);

        if (check_parity) {
            Serial.println("Parity bit checks out");
        } else {
            Serial.println("Parity bit doesn't check out");
        }

        sprintf(output, "Message[%u]: %c", message_count, current_char);
        Serial.println(output);

        message[message_count++] = current_char;

        current_position = 0;
        check_parity = false;
        current_char = 0;

        Serial.println();

        return;
    } else {
        // For any data bit, write it into the current character
        bitWrite(current_char, current_position - 1, received_bit);
        sprintf(output, "B[%u]: ", current_position - 1);
        Serial.print(output);
    }

    Serial.println(received_bit);

    current_position++;
}

void setup() {
    // Disable interrupts
    noInterrupts();

    // Setup serial port
    Serial.begin(9600);

    // Setup RX, RTS, and CTS pins
    pinMode(PIN_RX, INPUT);
    pinMode(PIN_RTS, INPUT);
    pinMode(PIN_CTS, OUTPUT);

    // Setup timer
    timer_setup(BAUD_RATE);

    // Enable interrupts
    interrupts();

    Serial.println("Setup done\n");
}

void loop() {
    if (digitalRead(PIN_RTS) == HIGH) {
        Serial.println("Start of transmission");

        digitalWrite(PIN_CTS, HIGH);

        timer_start();

        current_position = 0;
        check_parity = false;
        current_char = 0;

        while (digitalRead(PIN_RTS) == HIGH) {
            delay(1000);
        }

        timer_stop();

        digitalWrite(PIN_CTS, LOW);

        Serial.print("\nMessage: ");
        Serial.println(message);
        Serial.println();

        message_count = 0;
        memset(message, 0, sizeof(message));

        Serial.println("End of transmission\n");
    }
}
