#include <ae2f/Ann/Slp.h>

static void MlpTrain_OutCompute(
    const ae2f_AnnSlp* layerOut,
    const ae2f_float_t* goal,
    const ae2f_float_t* out,
    ae2f_float_t* retDeltaOut
) {
    for(size_t i = 0; i < layerOut->outc; i++) {
        retDeltaOut[i] = ae2f_AnnSlpPerV(layerOut, i, const)->CalDelta(
            out[i], goal[i]
        );
    }
}