#include "predictor.h"

PREDICTOR::PREDICTOR() : global_history(0) {
    memset(pht, 0, sizeof(pht)); // Initialize the PHT entries to 0
}

bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os) {
    uint32_t index = (br->instruction_addr ^ global_history) & PHT_INDEX_MASK; // Calculate PHT index
    uint8_t counter = pht[index]; // Get the 2-bit counter value
    return (counter >> 1) != 0; // Predict 'taken' if counter's high bit is set
}

void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken) {
    uint32_t index = (br->instruction_addr ^ global_history) & PHT_INDEX_MASK; // Calculate PHT index
    uint8_t counter = pht[index]; // Get the 2-bit counter

    // Increment or decrement the counter based on the branch outcome
    if(taken) {
        if(counter < 3) counter++;
    } else {
        if(counter > 0) counter--;
    }
    pht[index] = counter; // Update the counter in PHT

    // Update global history with the most recent branch outcome
    global_history = (global_history << 1) | (taken ? 1 : 0);
    global_history &= (1 << HISTORY_LENGTH) - 1; // Keep only the relevant bits
}

