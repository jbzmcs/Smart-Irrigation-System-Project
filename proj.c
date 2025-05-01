#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define SOIL_SENSOR_CHANNEL 0
#define RELAY_PIN PD2
#define THRESHOLD 512

void adc_init(void) {
    ADMUX = (1 << REFS0); // AVcc ref
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Prescaler 64
}

uint16_t read_adc(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void gpio_init(void) {
    DDRD |= (1 << RELAY_PIN);
}

void timer1_init(void) {
    // CTC mode, prescaler 1024, OCR1A for 500ms interval
    TCCR1B |= (1 << WGM12); // CTC mode
    OCR1A = 7812; // 16MHz / 1024 / 2Hz = 7812
    TIMSK1 |= (1 << OCIE1A); // Enable compare match interrupt
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
}

ISR(TIMER1_COMPA_vect) {
    uint16_t moisture = read_adc(SOIL_SENSOR_CHANNEL);
    if (moisture < THRESHOLD)
        PORTD |= (1 << RELAY_PIN);
    else
        PORTD &= ~(1 << RELAY_PIN);
}

int main(void) {
    adc_init();
    gpio_init();
    timer1_init();
    sei(); // Enable global interrupts

    while (1) {
        // MCU can sleep or do other tasks
    }
}