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
#include <ann-h/Sp.h>
#include <ae2f/Ann/LcgRand.h>

ae2f_SHAREDEXPORT
size_t ae2f_mAnnSpInit(
    ae2f_mAnnSp* _this,
    size_t inc,
    ae2f_float_t* Field_opt,
    ae2f_fpAnnAct_t vAct,
    ae2f_fpAnnAct_t vActDerive,
    ae2f_fpAnnLoss_t vLossDeriv,
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
    _this->vLossDeriv = vLossDeriv;
    _this->vAct = vAct;
    _this->vActDeriv = vActDerive;
    _this->vPredict = Predict;
    _this->vTrain = Train;
    _this->vClean = 0;

    if(Field_opt) {
        _this->pField = Field_opt;
        offset_opt -= sizeof(ae2f_float_t) * (_this->inc + 1);
    } else {
        _this->pField = ae2f_reinterpret_cast(ae2f_float_t*, _this + 1);
        uint64_t _ = ae2f_AnnLcgRandSeed.u64;

        for(size_t i = 0; i < _this->inc; i++) {
            _ = ae2f_AnnLcgRand(_);
            ae2f_float_t d = ae2f_AnnLcgRandDistribute(_);
            ae2f_mAnnSpW(_this)[i] = d;
        }
        ae2f_AnnLcgRandSeed.u64 = _;
    }

    {
        uint64_t _ = ae2f_AnnLcgRandG();
        ae2f_AnnLcgRandSeed.u64 = _;
        *ae2f_mAnnSpB(_this) = ae2f_AnnLcgRandDistribute(_);
    }

    DONE:
    if(erret) *erret = er;
    return ae2f_mAnnSpInitSz(offset_opt, inc);
}

ae2f_SHAREDEXPORT
ae2f_AnnSp* ae2f_AnnSpMk(
    size_t inc,
    ae2f_float_t* Field_opt,
    ae2f_fpAnnAct_t vAct,
    ae2f_fpAnnAct_t vActDerive,
    ae2f_fpAnnLoss_t vLossDeriv,
    ae2f_err_t* erret,
    size_t additional
) {
    ae2f_AnnSp* _this = 0;

    _this = calloc(ae2f_mAnnSpInitSz(additional - (Field_opt ? (inc + 1) * sizeof(ae2f_float_t) : 0), inc), 1);
    ae2f_mAnnSpInit(&_this->Sp, inc, Field_opt, vAct, vActDerive, vLossDeriv, erret, additional);

    rtn:
    if(erret) *erret &= ~ae2f_errGlob_DONE_HOWEV;
    return _this;
}