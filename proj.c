/**
 * Smart Irrigation System
 * 
 * This system monitors soil moisture and automatically waters plants
 * when moisture levels drop below a defined threshold.
 * 
 * Hardware:
 * - Arduino Uno (ATmega328P)
 * - Soil moisture sensor (analog)
 * - Relay module (controlling water pump)
 * - Water pump
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

/* Pin and configuration definitions */
#define SOIL_SENSOR_CHANNEL 0    /* ADC channel for soil moisture sensor */
#define RELAY_PIN PD2            /* Digital pin for relay control */

/* System parameters */
#define MOISTURE_THRESHOLD 512   /* Threshold for triggering watering (0-1023) */
#define MAX_WATERING_TIME 10     /* Maximum watering time in seconds */
#define SAMPLING_INTERVAL 2000   /* Time between moisture readings in milliseconds */

/* System states */
volatile uint8_t is_watering = 0;         /* Flag to track if watering is active */
volatile uint32_t watering_start_time = 0; /* Track watering duration */
volatile uint32_t system_ticks = 0;       /* System timer ticks (milliseconds) */

/**
 * Initialize ADC for soil moisture reading
 */
void adc_init(void) {
    /* Use AVcc as reference voltage */
    ADMUX = (1 << REFS0);
    
    /* Enable ADC, set prescaler to 64 (250kHz ADC clock @ 16MHz) */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
    
    /* Perform a dummy reading to initialize ADC */
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
}

/**
 * Read analog value from specified ADC channel
 */
uint16_t read_adc(uint8_t channel) {
    /* Select ADC channel while preserving reference selection bits */
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    /* Start conversion and wait for completion */
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    
    /* Return 10-bit result */
    return ADC;
}

/**
 * Initialize GPIO pins
 */
void gpio_init(void) {
    /* Set relay pin as output */
    DDRD |= (1 << RELAY_PIN);
    
    /* Ensure relay is off initially */
    PORTD &= ~(1 << RELAY_PIN);
}

/**
 * Initialize Timer1 for regular moisture sampling
 */
void timer1_init(void) {
    /* Configure Timer1 for CTC mode */
    TCCR1B |= (1 << WGM12);
    
    /* Set compare value for ~100ms interval */
    /* 16MHz / 256 / 625 = 100ms */
    OCR1A = 625;
    
    /* Enable Timer1 compare match A interrupt */
    TIMSK1 |= (1 << OCIE1A);
    
    /* Start timer with 256 prescaler */
    TCCR1B |= (1 << CS12);
}

/**
 * Start watering cycle
 */
void start_watering(void) {
    is_watering = 1;
    watering_start_time = system_ticks;
    PORTD |= (1 << RELAY_PIN);  /* Turn on relay/pump */
}

/**
 * Stop watering cycle
 */
void stop_watering(void) {
    is_watering = 0;
    PORTD &= ~(1 << RELAY_PIN); /* Turn off relay/pump */
}

/**
 * Timer1 Compare Match A Interrupt Service Routine
 * - Runs every ~100ms to update system time
 * - Checks moisture every SAMPLING_INTERVAL
 * - Manages watering cycle safety timeouts
 */
ISR(TIMER1_COMPA_vect) {
    /* Update system time (milliseconds) */
    system_ticks += 100;
    
    /* Check if it's time to read moisture */
    if (system_ticks % SAMPLING_INTERVAL == 0) {
        uint16_t moisture = read_adc(SOIL_SENSOR_CHANNEL);
        
        /* Start watering if soil is too dry and not already watering */
        if (moisture < MOISTURE_THRESHOLD && !is_watering) {
            start_watering();
        }
        /* Stop watering if soil is moist enough */
        else if (moisture >= MOISTURE_THRESHOLD && is_watering) {
            stop_watering();
        }
    }
    
    /* Safety feature: limit maximum watering time */
    if (is_watering && (system_ticks - watering_start_time) >= (MAX_WATERING_TIME * 1000)) {
        stop_watering();
    }
}

/**
 * Main program entry point
 */
int main(void) {
    uint16_t initial_moisture;
    
    /* Initialize system components */
    adc_init();
    gpio_init();
    timer1_init();
    
    /* Enable global interrupts */
    sei();
    
    /* Initial moisture reading */
    initial_moisture = read_adc(SOIL_SENSOR_CHANNEL);
    
    /* Enter low-power mode if moisture is adequate */
    if (initial_moisture >= MOISTURE_THRESHOLD) {
        set_sleep_mode(SLEEP_MODE_IDLE);
    }
    
    /* Main application loop */
    while (1) {
        /* The system operates primarily through interrupts */
        /* MCU can enter sleep mode to save power */
        sleep_mode();
    }
    
    return 0; /* Never reached */
}