/* Include files are in /usr/avr/include/ */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "util.h"

#include <stdint.h>
#include <stdbool.h>

/*
 * <util/delay.h>
 * _delay_ms(double ms)
 * _delay_us(double us)
 */

/* Interrupt vectors
 * RESET           External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset
 * INT0            External Interrupt Request 0
 * PCINT0          Pin Change Interrupt Request 0
 * TIMER1_COMPA    Timer/Counter1 Compare Match A
 * TIMER1_OVF      Timer/Counter1 Overflow
 * TIMER0_OVF      Timer/Counter0 Overflow
 * EE_RDY          EEPROM Ready
 * ANA_COMP        Analog Comparator
 * ADC             ADC Conversion Complete
 * TIMER1_COMPB    Timer/Counter1 Compare Match B
 * TIMER0_COMPA    Timer/Counter0 Compare Match A
 * TIMER0_COMPB    Timer/Counter0 Compare Match B
 * WDT             Watchdog Time-out
 * USI_START       USI START
 * USI_OVF         USI Overflow
*/

/*
 * Port B pins: 0, 1, 2, 3, 4, (5 == RESET)
 * PB0 (PCINT0/AIN0/OC0A/OC1A/MOSI/DI/SDA/AREF) : Physical pin 5
 * PB1 (PCINT1/AIN1/OC0B/OC1A/MISO/DO)          : Physical pin 6
 * PB2 (PCINT2/ADC1/SCK/USCK/SCL/T0/INT0)       : Physical pin 7
 * PB3 (PCINT3/ADC3/OC1B/XTAL1/CLKI)            : Physical pin 2
 * PB4 (PCINT4/ADC2/OC1B/XTAL2/CLKO)            : Physical pin 3
 * PB5 (PCINT5/ADC0/RESET/dW)                   : Physical pin 1
 *
 *                                      VCC     : Physical pin 8
 *                                      GND     : Physical pin 4
 */

static void _init_port_b(uint8_t outputs, uint8_t initial_value)
{
    PORTB = initial_value;
    DDRB = outputs;
}

static inline bool _b0(void)
{
    return ((PORTB & PB0) != 0);
}

typedef enum {
    WDT_PERIOD_MS_16,
    WDT_PERIOD_MS_32,
    WDT_PERIOD_MS_64,
    WDT_PERIOD_MS_128,
    WDT_PERIOD_MS_256,
    WDT_PERIOD_MS_512,
    WDT_PERIOD_MS_1024,
    WDT_PERIOD_MS_2048,
    WDT_PERIOD_MS_4096,
    WDT_PERIOD_MS_8192,
    WDT_PERIOD_MS_INVALID
} wdt_period_t;

/*
 * Timers:
 *
 * Timer 0 - 8 bit with glitch free PWM
 * Timer 1 - 8 bit with PWM - separate prescaler - can use PLL at 64MHz
 * Watchdog timer - period (typ): 16/32/64/128/256/512/1024/2048/4096/8192ms
 */
static uint8_t _wdt_tick = 0;
static void _init_wdt_timer(wdt_period_t period)
{
    if (period < WDT_PERIOD_MS_INVALID)
    {
        WDTCR = WDIE | ((period & 0x08) ? WDP3 : 0) | (period & 0x7);
    }

}

/* Not used yet
 * There is a special procedure to disable the watchdog timer
static void _disable_wdt_timer(void)
{
    if (WDTCR & WDE)
    {
        WDTCR = WDCE | WDE;
        WDTCR = 0;
    }
}
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
    _init_port_b(DDB0 | DDB1, PB1 | PB2);

    // Set up the timer to generate a periodic interrupt
    _init_wdt_timer(WDT_PERIOD_MS_64);

    GIMSK = 0b00100000; // Enable pin-change interrupt
    PCMSK = 0b00000000; // Enable pin change on no pins...
    sei();      // Enable global interrupts
}

static uint8_t read_inputs(void)
{
    uint8_t inputs = 0;

    // Read inputs and map to a bitmap
    inputs = _b0() ? 1 : 0;

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
        uint8_t last_tick = 0xff;

        if (last_tick != _wdt_tick)
        {
            last_tick = _wdt_tick;      // Update tick match
            uint8_t inputs = read_inputs();

            // Do processing
            process(inputs, &out_set, &out_clear);

            set_outputs(out_set, out_clear);
        }
    }

    // _disable_wdt_timer();
}

/*
 * Pin change interrupt vector
 */
ISR (PCINT0_vect)
{
    // Do something on pin change
}

/*
 * Interrupt vectors for timer 0
 */
ISR (TIMER0_OVF_vect)
{
}

ISR (TIMER0_COMPA_vect)
{
}

ISR (TIMER0_COMPB_vect)
{
}

/*
 * Interrupt vector for watchdog timer (WDT) time-out
 */
ISR(WDT_vect)
{
    WDTCR |= WDIE;      // Reset the interrupt enable flag to prevent reset
    _wdt_tick++;        // Count ticks
}
