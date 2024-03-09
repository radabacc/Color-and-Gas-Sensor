#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> 

// Pin assignments for the TCS3200 pins
#define S0_PIN PB0 // Arduino Pin 8
#define S1_PIN PD7 // Arduino Pin 8
#define S2_PIN PD6 // Arduino Pin 8
#define S3_PIN PD5 // Arduino Pin 8
#define sensorOut_PIN PD3 // Arduino Pin 8

// Pin assignments for the LEDs
#define redLedPin PB4 // Arduino Pin 8
#define greenLedPin PB3 // Arduino Pin 8
#define blueLedPin PB5 // Arduino Pin 8

// Function prototypes
unsigned int readColor(int s2, int s3);

int main() {
    // Initialize sensor pins
    DDRB |= (1 << S0_PIN);
    DDRD |= (1 << S1_PIN) | (1 << S2_PIN) | (1 << S3_PIN);
    DDRD &= ~(1 << sensorOut_PIN); // Set sensorOut as input

    // Set frequency scaling to 100%
    PORTB |= (1 << S0_PIN);
    PORTD |= (1 << S1_PIN);

    // Configure LED pins as outputs
    DDRB |= (1 << redLedPin) | (1 << greenLedPin) | (1 << blueLedPin);

    // Set up Timer1 for pulse width measurement
    TCCR1B |= (1 << CS10); 

    while (1) {
    // Read the pulse widths for each color
    unsigned int redPulse = readColor(0, 0);     // Red
    unsigned int greenPulse = readColor(1, 1);   // Green
    unsigned int bluePulse = readColor(0, 1);    // Blue

    // Check for a threshold of normal room light
    if (bluePulse > 300 && greenPulse > 300 && redPulse > 1) {
        // Turn off all LED's in normla room light
        PORTB &= ~((1 << redLedPin) | (1 << greenLedPin) | (1 << blueLedPin));
    } else {
        // Turn off all LEDs before setting the dominant color
        PORTB &= ~((1 << redLedPin) | (1 << greenLedPin) | (1 << blueLedPin));
        // Determine the dominant color
        if (redPulse < greenPulse && redPulse < bluePulse) {
            // Red is dominant
            PORTB |= (1 << redLedPin);
        } else if (greenPulse < redPulse && greenPulse < bluePulse) {
            // Green is dominant
            PORTB |= (1 << greenLedPin);
        } else if (bluePulse < redPulse && bluePulse < greenPulse) {
            // Blue is dominant
            PORTB |= (1 << blueLedPin);
        }
    }
    _delay_ms(1000);
    } 
    return 0;
}

// Function to read color pulse widths
unsigned int readColor(int s2, int s3) {
    PORTD = (PORTD & ~((1 << S2_PIN) | (1 << S3_PIN))) | (s2 << S2_PIN) | (s3 << S3_PIN);

    // Wait for the output to go low
    while (!(PIND & (1 << sensorOut_PIN)));

    // Start the timer
    TCNT1 = 0;

    // Wait for the output to go high
    while (PIND & (1 << sensorOut_PIN));

    // Read the timer value
    unsigned int pulseWidth = TCNT1;

    return pulseWidth;
}
