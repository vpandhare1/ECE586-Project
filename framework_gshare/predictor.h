#ifndef PREDICTOR_H_SEEN
#define PREDICTOR_H_SEEN

#include <cstddef>
#include <cstring>
#include <inttypes.h>
#include <vector>
#include "op_state.h"
#include "tread.h"

// Define the length of the branch history
#define HISTORY_LENGTH 14
// Set the size of the Pattern History Table (PHT) based on HISTORY_LENGTH
#define PHT_SIZE (1 << HISTORY_LENGTH)
// Define a mask to calculate the index in PHT
#define PHT_INDEX_MASK (PHT_SIZE - 1)

class PREDICTOR {
private:
    uint16_t global_history; // Holds the global branch history
    uint8_t pht[PHT_SIZE]; // Pattern History Table for branch prediction

public:
    PREDICTOR(); // Constructor initializes the predictor
    bool get_prediction(const branch_record_c* br, const op_state_c* os); // Predicts branch direction
    void update_predictor(const branch_record_c* br, const op_state_c* os, bool taken); // Updates predictor state
};

#endif // PREDICTOR_H_SEEN

