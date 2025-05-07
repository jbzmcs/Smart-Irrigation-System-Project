/**
 * Relay Control Module Implementation
 * Controls the water pump relay
 */
#include "relay.h"

/**
 * Initialize relay pin as output
 */
void Relay_init(void) {
    // Set relay pin as output
    RELAY_DDR |= (1 << RELAY_PIN);
    
    // Ensure relay starts in OFF state (relay is active LOW)
    RELAY_PORT |= (1 << RELAY_PIN);
}

/**
 * Turn relay on (activate water pump)
 * Relay is active LOW, so we clear the pin
 */
void Relay_on(void) {
    RELAY_PORT &= ~(1 << RELAY_PIN);
}

/**
 * Turn relay off (deactivate water pump)
 * Relay is active LOW, so we set the pin high
 */
void Relay_off(void) {
    RELAY_PORT |= (1 << RELAY_PIN);
}

/**
 * Toggle relay state
 */
void Relay_toggle(void) {
    RELAY_PORT ^= (1 << RELAY_PIN);
}