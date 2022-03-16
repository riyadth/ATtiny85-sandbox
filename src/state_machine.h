/*
 * Simple state machine
 */

#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include <stdint.h>

typedef uint8_t state_id_t;
typedef state_id_t (*state_machine_exec_t)(void);
typedef void (*state_machine_entry_t)(void);
typedef void (*state_machine_exit_t)(void);

typedef struct {
    state_machine_entry_t entry;
    state_machine_exec_t exec;
    state_machine_exit_t exit;
} state_functions_t;

void sm_tick(void);

#endif // _STATE_MACHINE_H
