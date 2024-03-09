#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN  PB1 // Pin 9 on Arduino Uno
#define ALCOHOL_SENSOR_PIN PC0 // Analog pin 0 on Arduino Uno

void setup() {
    DDRB |= (1 << LED_PIN); // Set LED pin as output
    // Enable ADC
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // Set reference voltage
    ADMUX |= (1 << REFS0);
}

uint16_t readAlcoholSensor() {
    //  ADC channel for alcohol sensor
    ADMUX = (ADMUX & 0xF0) | (ALCOHOL_SENSOR_PIN & 0x0F);
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);
    // Wait for conversion
    while (ADCSRA & (1 << ADSC));
    // Return ADC value
    return ADC;
}

int main(void) {
    setup();
    
    while (1) {
        uint16_t sensorValue = readAlcoholSensor();
        if (sensorValue > 700) { // Threshold value
            PORTB |= (1 << LED_PIN); // Turn on LED
        } else {
            PORTB &= ~(1 << LED_PIN); // Turn off LED
        }
        
        _delay_ms(100); // Delay 
    }

    return 0;
}
