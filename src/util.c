/* Include files are in /usr/avr/include/ */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

/*
 * ATtiny85 utility functions
 */

#include "util.h"

#include <stdint.h>

reset_reason_t reset_reason(void)
{
    reset_reason_t reason;
    uint8_t raw_reason = MCUSR;

    if (raw_reason & RESET_POR) {
        reason = RESET_POR;
    }
    else if (raw_reason & RESET_EXT) {
        reason = RESET_EXT;
    }
    else if (raw_reason & RESET_BOR) {
        reason = RESET_BOR;
    }
    else if (raw_reason & RESET_WDT) {
        reason = RESET_WDT;
    }
    else {
        reason = RESET_UNK;
    }
    MCUSR = 0;

    return reason;
}

