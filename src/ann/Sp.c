/**
 * @file Sp.c
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdlib.h>
#include <memory.h>
#include "Sp.h"

static ae2f_AnnSpPredict_t Predict;
static ae2f_AnnSpTrain_t Train;

#include <ae2f/Ann/LcgRand.h>

ae2f_SHAREDEXPORT
size_t ae2f_AnnSpInit(
    ae2f_AnnSp* _this,
    size_t inc,
    const ae2f_float_t* W_opt,
    ae2f_fpAnnAct_t Act,
    ae2f_fpAnnDelta_t CalDelta,
    ae2f_err_t* erret,
    size_t offset_opt
) noexcept {
    ae2f_err_t er = 0;
    if(!_this) {
        er = ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_DONE_HOWEV;
        goto DONE;
    }
    if(!inc) {
        er = ae2f_errGlob_WRONG_OPERATION;
        goto DONE;
    }

    _this->expected = 1;
    _this->inc = inc;
    _this->CalDelta = CalDelta;
    _this->Act = Act;
    _this->vPredict = Predict;
    _this->vTrain = Train;
    _this->vClean = 0;

    if(W_opt) {
        memcpy(ae2f_AnnSpW(_this), W_opt, sizeof(ae2f_float_t) * inc);
    } else {
        uint64_t _ = ae2f_AnnLcgRandSeed.u64;

        for(size_t i = 0; i < _this->inc; i++) {
            _ = ae2f_AnnLcgRandG();
            ae2f_float_t d = ae2f_AnnLcgRandDistribute(_);
            ae2f_AnnSpW(_this)[i] = d;
        }

        ae2f_AnnLcgRandSeed.u64 = _;
    }

    DONE:
    if(erret) *erret = er;
    return ae2f_AnnSpInitSz(offset_opt, inc);
}

ae2f_SHAREDEXPORT
ae2f_AnnSp* ae2f_AnnSpMk(
    size_t inc,
    const ae2f_float_t* W_opt,
    ae2f_fpAnnAct_t Act,
    ae2f_fpAnnDelta_t CalDelta,
    ae2f_err_t* erret,
    size_t additional
) {
    ae2f_AnnSp* _this = 0;

    _this = calloc(ae2f_AnnSpInitSz(additional, inc), 1);
    ae2f_AnnSpInit(_this, inc, W_opt, Act, CalDelta, erret, additional);

    rtn:
    if(erret) *erret &= ~ae2f_errGlob_DONE_HOWEV;
    return _this;
}

static
ae2f_err_t Predict(
    const ae2f_AnnSp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) {
    if(!in) return (ae2f_errGlob_PTR_IS_NULL);
    if(!_this) return (ae2f_errGlob_PTR_IS_NULL);
    if(!out_opt) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;

    ae2f_float_t sum = 0;
    for(size_t i = 0; i < _this->inc; i++) {
        sum += ae2f_AnnSpPredictI(_this, in, i);
    }

    if(_this->Act) sum = (_this)->Act(sum + *ae2f_AnnSpB(_this));

    __DONE:
    if(out_opt) *out_opt = sum;
    return ae2f_errGlob_OK;
}

static
ae2f_err_t Train(
    ae2f_AnnSp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    ae2f_float_t goal_optB,
    ae2f_float_t learningrate
) {
    ae2f_err_t err = ae2f_errGlob_OK;
    ae2f_float_t _delta = 0;

    if(!_this) return (ae2f_errGlob_PTR_IS_NULL);
    if(!in) return (ae2f_errGlob_PTR_IS_NULL);
    if(learningrate == 0) return (ae2f_errGlob_OK);
    if(!_this->CalDelta) return ae2f_errGlob_IMP_NOT_FOUND;

    err = ae2f_AnnSpPredict(_this, in, &_delta);

    if(delta_optA) 
    _delta = *delta_optA;
    else 
    _delta = _this->CalDelta(_delta, goal_optB);

    _delta *= learningrate;
    for(size_t i = 0; i < _this->inc; i++) {
        ae2f_AnnSpW(_this, )[i] += _delta * in[i];
    }

    __DONE:
    return err;
}
