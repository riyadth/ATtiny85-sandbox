/* Include files are in /usr/avr/include/ */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "util.h"

/*
 * <util/delay.h>
 * _delay_ms(double ms)
 * _delay_us(double us)
 */

const uint8_t table[] PROGMEM = {
    0x00, 0x01, 0x02, 0x03, 0x04
};

const char _hex[] PROGMEM = "0123456789abcdef";

char *hex_byte(uint8_t byte)
{
    static char result[3];
    uint8_t idx=0;

    result[idx++] = pgm_read_byte(&(_hex[((byte >> 4) & 0xf)]));
    result[idx++] = pgm_read_byte(&(_hex[((byte >> 0) & 0xf)]));
    result[idx] = '\0';

    return result;
}

char *hex_word(uint16_t word)
{
    static char result[5];
    uint8_t idx=0;

    result[idx++] = pgm_read_byte(&(_hex[((word >> 12) & 0xf)]));
    result[idx++] = pgm_read_byte(&(_hex[((word >> 8) & 0xf)]));
    result[idx++] = pgm_read_byte(&(_hex[((word >> 4) & 0xf)]));
    result[idx++] = pgm_read_byte(&(_hex[((word >> 0) & 0xf)]));
    result[idx] = '\0';

    return result;
}

static void hardware_setup(void)
{
    cli();      // Disable global interrupts

    // Set pin configuration and initial output values

    GIMSK = 0b00100000; // Enable pin-change interrupt
    PCMSK = 0b00000000; // Enable pin change on no pins...
    sei();      // Enable global interrupts
}

static uint8_t read_inputs(void)
{
    uint8_t inputs = 0;

    // Read inputs and map to a bitmap

    return inputs;
}

static void set_outputs(uint8_t sets, uint8_t clears)
{
    // Set and clear output pins as requested
}

static void process(uint8_t in, uint8_t *sets, uint8_t *clears)
{
    // Process the input and come up with new outputs
}

int main(void)
{
    reset_reason_t reason = reset_reason();

    switch (reason) {
        case RESET_POR:
            break;
        case RESET_EXT:
            break;
        default:
    }

    hardware_setup();

    while (1)
    {
        uint8_t out_set = 0;
        uint8_t out_clear = 0;
        uint8_t inputs = read_inputs();

        // Do processing
        process(inputs, &out_set, &out_clear);

        set_outputs(out_set, out_clear);
    }
}

/*
 * Pin change interrupt vector
 */
ISR (PCINT0_vect) {
    // Do something on pin change
}
