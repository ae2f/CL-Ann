#include <ae2f/Ann/Slp.h>
#include <stdlib.h>

static ae2f_mAnnSlpPredict_t Predict;
static ae2f_mAnnSlpTrain_t Train;
static ae2f_mAnnSlpClean_t Clean;

static ae2f_err_t Predict(
    const ae2f_mAnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) noexcept {
	if(!_this) return ae2f_errGlob_PTR_IS_NULL;
	if(!out_opt) return ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL;
	if(!in) return ae2f_errGlob_PTR_IS_NULL;
	
	ae2f_err_t err = 0;

	for(size_t i = 0; i < _this->outc; i++) {
		union {
			const size_t* pad;
			const ae2f_mAnnSp* perc;
		}  layer = { ae2f_mAnnSlpPerVPad(_this, const)[i] };

		size_t _pad = *layer.pad;
		layer.pad++;

		ae2f_err_t er = 
			ae2f_mAnnSpPredict(
					layer.perc,
					in + _pad,
					out_opt + i
					);

		err |= er;
	}

	return err;
}

static ae2f_err_t Train(
    ae2f_mAnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(learningrate == 0) return ae2f_errGlob_OK;
    if(!_this->vPredict) return ae2f_errGlob_IMP_NOT_FOUND;

    ae2f_err_t _er = 0;

    _er = _this->vPredict(_this, in, ae2f_mAnnSlpOutCache(_this));
    if(_er) return _er;

    __update:
    for(size_t i = 0; i < _this->outc; i++) {
        size_t _pad = *(ae2f_mAnnSlpPerVPad(_this, const)[i]);
        ae2f_mAnnSp* perc = ae2f_mAnnSlpPerV(_this, i);

        ae2f_mAnnSlpOutCache(_this)[i] = 
        delta_optA ?
        delta_optA[i] :
        perc->vLossDeriv(ae2f_mAnnSlpOutCache(_this), goal_optB, i, _this->outc)
        * perc->vActDeriv(ae2f_mAnnSlpOutCache(_this)[i]) 
        * learningrate; /**/

        _er |= ae2f_mAnnSpTrain(
            perc, 
            in + _pad, 
            ae2f_mAnnSlpOutCache(_this) + i, 
            0, 
            learningrate
        );
    }

    return _er;
}

static ae2f_err_t Clean(
    ae2f_mAnnSlp* _this
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;

    for(size_t i = 0; i < _this->outc; i++) {
        const union {
            ae2f_mAnnSp* v;
            size_t* S;
        } per = { .v = ae2f_mAnnSlpPerV(_this, i)};
        
        ae2f_mAnnSpClean(per.v);
        free(ae2f_mAnnSlpPerVPad(_this)[i]);
        
        ae2f_mAnnSlpPerVPad(_this, )[i] = 0;
    }

    _this->inc = 0;
    _this->outc = 0;
    _this->layerc = 0;
    _this->vClean = 0;
    _this->vPredict = 0;
    _this->vTrain = 0;

    return ae2f_errGlob_OK;
}
