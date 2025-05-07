/**
 * Timer Module Header
 * Handles timing for periodic moisture sensing
 */
#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

/**
 * Initialize Timer1 for periodic moisture sensing
 * Sets up timer to overflow approximately every 1 second
 */
void Timer1_init(void);

#endif /* TIMER_H */