#ifndef PREDICTOR_H_SEEN
#define PREDICTOR_H_SEEN

#include <cstddef>
#include <cstring>
#include <inttypes.h>
#include <vector>
#include "op_state.h"
#include "tread.h"
#include <cstdint> // For uint32_t, uint8_t, etc.

// Perceptron predictor constants
#define NUM_PERCEPTRONS 256 // Number of perceptrons to fit within memory constraints
#define HISTORY_LENGTH 12 // History length used by perceptrons
#define THRESHOLD 1.93 * HISTORY_LENGTH + 14 // Threshold for training perceptrons, as per literature

// Structure to represent a perceptron
struct Perceptron {
    int weights[HISTORY_LENGTH + 1]; // +1 for bias weight
};

class PREDICTOR {
public:
    Perceptron perceptrons[NUM_PERCEPTRONS];
    uint32_t globalHistory; // Global history register

    PREDICTOR();
    bool get_prediction(const branch_record_c* br, const op_state_c* os);
    void update_predictor(const branch_record_c* br, const op_state_c* os, bool taken);

private:
    int8_t historyBits[HISTORY_LENGTH]; // History bits for perceptron input
    void updateGlobalHistory(bool taken);
    int hashPC(uint32_t pc);
};

#endif // PREDICTOR_H_SEEN

