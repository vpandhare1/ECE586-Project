#include "predictor.h"


const int GLOBAL_HISTORY_BITS = 12;
const int LOCAL_HISTORY_BITS = 10;
const int PC_INDEX_BITS = 10;  // Using PC[2:11] for local history indexing

// Global variables for histories and prediction tables
uint32_t globalHistory = 0;
uint32_t localHistory[1 << PC_INDEX_BITS];
uint8_t globalPHT[1 << GLOBAL_HISTORY_BITS];
uint8_t localPHT[1 << LOCAL_HISTORY_BITS];
uint8_t choicePHT[1 << GLOBAL_HISTORY_BITS];

// Initializing the Pattern History Table (PHT)
void initPHT(uint8_t* pht, int size, uint8_t initialValue) {
    for (int i = 0; i < size; ++i) {
        pht[i] = initialValue;  // This will Set each entry to the initial value
    }
}

// Update the saturating counter in PHT
void updateCounter(uint8_t& counter, bool taken) {
    if (taken && counter < 3) counter++;  // Increment for taken
    else if (!taken && counter > 0) counter--;  // Decrement for not taken
}


PREDICTOR::PREDICTOR() {
    globalHistory = 0;  
    // Initialize PHTs: Global and Choice to strongly taken (3), Local to weakly not taken (1)
    initPHT(globalPHT, 1 << GLOBAL_HISTORY_BITS, 3);
    initPHT(localPHT, 1 << LOCAL_HISTORY_BITS, 1);
    initPHT(choicePHT, 1 << GLOBAL_HISTORY_BITS, 3);
    // Clear local history table
    memset(localHistory, 0, sizeof(localHistory));
}


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os) {
    if (!br->is_conditional) return false; 

    // Index local history using PC[2:11]
    uint32_t pcIndex = (br->instruction_addr >> 2) & ((1 << PC_INDEX_BITS) - 1);
    uint32_t localHistIndex = localHistory[pcIndex];
    bool localPrediction = (localPHT[localHistIndex] >> 1) == 1;

    // Use global history for global prediction
    uint32_t globalHistIndex = globalHistory & ((1 << GLOBAL_HISTORY_BITS) - 1);
    bool globalPrediction = (globalPHT[globalHistIndex] >> 1) == 1;

    // Decide between local and global prediction
    uint32_t choiceIndex = globalHistIndex;
    bool useGlobal = (choicePHT[choiceIndex] >> 1) == 1;

    return useGlobal ? globalPrediction : localPrediction;
}


void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken) {
    // Index local history using PC[2:11]
    uint32_t pcIndex = (br->instruction_addr >> 2) & ((1 << PC_INDEX_BITS) - 1);
    uint32_t localHistIndex = localHistory[pcIndex];

    // Update local and global predictions and histories
    updateCounter(localPHT[localHistIndex], taken);
    updateCounter(globalPHT[globalHistory & ((1 << GLOBAL_HISTORY_BITS) - 1)], taken);

    // Update choice predictor if predictions are not equal 
    bool localCorrect = (localPHT[localHistIndex] >> 1) == taken;
    bool globalCorrect = (globalPHT[globalHistory & ((1 << GLOBAL_HISTORY_BITS) - 1)] >> 1) == taken;
    if (localCorrect != globalCorrect) {
        updateCounter(choicePHT[globalHistory & ((1 << GLOBAL_HISTORY_BITS) - 1)], globalCorrect);
    }

    // Update global history
    globalHistory = (globalHistory << 1 | taken) & ((1 << GLOBAL_HISTORY_BITS) - 1);
    // Update local history
    localHistory[pcIndex] = (localHistIndex << 1 | taken) & ((1 << LOCAL_HISTORY_BITS) - 1);
}

