/*
 * ATtiny85 utility functions
 */

#ifndef ATTINY_UTILITY
#define ATTINY_UTILITY

typedef enum {
    RESET_UNK = 0,
    RESET_POR = 1 << PORF,
    RESET_EXT = 1 << EXTRF,
    RESET_BOR = 1 << BORF,
    RESET_WDT = 1 << WDRF
} reset_reason_t;

reset_reason_t reset_reason(void);

#endif // ATTINY_UTILITY
