#ifndef PREDICTOR_H_SEEN
#define PREDICTOR_H_SEEN

#include <cstddef>
#include <cstring>
#include <inttypes.h>
#include <vector>
#include "op_state.h"   // defines op_state_c (architectural state) class 
#include "tread.h"      // defines branch_record_c class

#define BHT_SIZE 2048
#define LHT_SIZE 1024
#define LPT_SIZE 1024
#define META_SIZE 2048
#define BHT_INDEX_MASK (BHT_SIZE - 1)
#define LHT_INDEX_MASK (LHT_SIZE - 1)
#define LPT_INDEX_MASK (LPT_SIZE - 1)
#define META_INDEX_MASK (META_SIZE - 1)
#define GHR_SIZE 16
#define LHR_SIZE 16

class PREDICTOR {
    uint8_t BHT[BHT_SIZE]; // GShare Branch History Table
    uint8_t LPT[LPT_SIZE]; // Local Prediction Table
    uint32_t LHT[LHT_SIZE]; // Local History Table, storing histories for each local branch
    uint8_t META[META_SIZE]; // Meta Predictor to choose between GShare and Local
    uint32_t GHR; // Global History Register, storing the history of the last GHR_SIZE branches

public:
    PREDICTOR(); // Constructor to initialize tables and register
    bool get_prediction(const branch_record_c* br, const op_state_c* os);
    void update_predictor(const branch_record_c* br, const op_state_c* os, bool taken);
};

#endif // PREDICTOR_H_SEEN

