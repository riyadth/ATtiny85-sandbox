/*
 * Simple state machine
 */

#include "state_machine.h"

#include <assert.h>

static void init_entry(void)
{
}

static state_id_t init_exec(void)
{
    state_id_t state = 0;

    return state;
}

static void init_exit(void)
{
}

static state_functions_t _state_table[] = {
    [0] = { .entry = init_entry, .exec = init_exec, .exit = init_exit },
};

// The state machine always starts in state 0, and no entry function is run
static state_id_t _state = (state_id_t)0;

void sm_tick(void)
{
    state_id_t new_state = _state_table[_state].exec();
    /*
    if (!_valid_state(new_state))
    {
        new_state = 0;
    }
    */
    if (new_state != _state)
    {
        if (_state_table[_state].exit != NULL)
        {
            _state_table[_state].exit();
        }
        _state = new_state;
        if (_state_table[_state].entry != NULL)
        {
            _state_table[_state].entry();
        }

        // Track time-in-state
    }
}
