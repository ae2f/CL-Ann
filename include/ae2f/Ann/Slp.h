/**
 * @file Slp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2f_Ann_Slp_h
#define ae2f_Ann_Slp_h

#include "./Sp.h"
#include <ae2f/Pack/Beg.h>

struct ae2f_mAnnSlp;

/// @brief cleaning function
typedef ae2f_err_t ae2f_mAnnSlpClean_t (ae2f_struct ae2f_mAnnSlp*) noexcept;

/// @brief 
/// Predict one output from multiple inputs.
/// @param[in] in 
/// Input vector. \n
/// Its count is @ref ae2f_mAnnSp::inc.
/// @param[out] outret_opt 
/// That one predicted. \n
/// Its count is @ref ae2f_mAnnSp::inc.
/// @return  State
typedef ae2f_err_t ae2f_mAnnSlpPredict_t (
    const ae2f_struct ae2f_mAnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* outret_opt
) noexcept;

/// @brief 
/// Training function, multiple input from multiple output.
/// @param[in] in 
/// Input vector. \n
/// Its count is @ref ae2f_mAnnSp::inc.
/// @param[in] delta_optA 
/// Delta vector when you pre-calculated it.
/// Its count is one.
/// @param[in] goal_optB 
/// Desired output.
/// @param learningrate 
/// Learning rate
typedef ae2f_err_t ae2f_mAnnSlpTrain_t (
    ae2f_struct ae2f_mAnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept;

/// @brief 
/// The element type for @ref ae2f_mAnnSlp.
typedef struct ae2f_mAnnSp ae2f_mAnnSlpEl;

/// @brief 
/// # Single Layered Perceptron
/// 
/// Multiple input, multiple output. \n
/// For predicting & training operations are able to be parallel.
typedef struct ae2f_mAnnSlp {

    /// @brief 
    /// True when its shape is same as expected. \n
    /// Expected structure is generated by @ref ae2f_mAnnSpInit.
    int expected;

    /// @brief 
    /// output count
    size_t outc;

    /// @brief 
    /// input count
    size_t inc;

    /// @brief 
    /// layer count
    size_t layerc;

    /// @brief 
    /// Cleaning function
    ae2f_mAnnSlpClean_t* vClean;

    /// @brief
    /// Predict function
    ae2f_mAnnSlpPredict_t* vPredict;

    /// @brief
    /// Training function
    ae2f_mAnnSlpTrain_t* vTrain;

    #if ae2f_WhenCXX(!)0
    #include "Slp.h.cxx/mSlp.hh"
    #endif
} ae2f_mAnnSlp;

/// @memberof ae2f_mAnnSlp
/// @brief
/// Input padding for perceptron.
/// 
/// Valid when @ref ae2f_mAnnSp::expected is 1. \n
/// When not, it is null.
#define ae2f_mAnnSlpPerVPad(slp, ...) \
(ae2f_CmpGetMem(slp, expected, 0) ? \
ae2f_reinterpret_cast(__VA_ARGS__ size_t* __VA_ARGS__ *, ae2f_static_cast(__VA_ARGS__ ae2f_mAnnSlp*, slp) + 1) : \
0)

/// @memberof ae2f_mAnnSlp
/// @brief
/// Perceptron vector. \n
/// Its length is same as @ref ae2f_mAnnSlp::outc.
///
/// Valid when @ref ae2f_mAnnSlp::expected is 1. \n
/// When not, it is null.
#define ae2f_mAnnSlpPerV(slp, i, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ ae2f_mAnnSlpEl*, (ae2f_mAnnSlpPerVPad(slp, __VA_ARGS__)[i] + 1))

/// @memberof ae2f_mAnnSlp
/// @brief
/// Additional buffer allocated.
#define ae2f_mAnnSlpX(slp, type, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ type, ae2f_CmpGetMem(slp, expected, 0) ? (ae2f_mAnnSlpPerVPad(slp, __VA_ARGS__) + (slp)->layerc) : 0)

/// @memberof ae2f_mAnnSlp
/// @brief
/// Desired least byte size for initialising.
#define ae2f_mAnnSlpInitSz(outc, off) ((off) + sizeof(ae2f_mAnnSlp) + (outc) * sizeof(void*))

/// @memberof ae2f_mAnnSlp
/// @brief
/// Caller of @ref ae2f_mAnnSlpPredict_t.
#define ae2f_mAnnSlpPredict ae2f_mAnnSpPredict

/// @memberof ae2f_mAnnSlp
/// @brief
/// Caller of @ref ae2f_mAnnSlpTrain_t.
#define ae2f_mAnnSlpTrain ae2f_mAnnSpTrain

/// @memberof ae2f_mAnnSlp
/// @brief 
/// Caller of @ref ae2f_mAnnSlpTrain_t \n
/// Train with Pre-calculated delta.
#define ae2f_mAnnSlpTrainA ae2f_mAnnSpTrainA

/// @memberof ae2f_mAnnSlp
/// @brief 
/// Caller of @ref ae2f_mAnnSlpTrain_t \n
/// Train for desired output: goal.
#define ae2f_mAnnSlpTrainB ae2f_mAnnSpTrainB

/// @memberof ae2f_mAnnSlp
/// @brief Cleaning function
#define ae2f_mAnnSlpClean ae2f_mAnnSpClean

/// @memberof ae2f_mAnnSlp
/// @brief 
/// @param[out] _this 
/// Allocated raw vector.
/// @param[in] incs_optA 
/// Desired Input Counts for each perceptron.
/// @param ginc_optB
/// Global Input Count
/// @param[in] inpads_opt 
/// Desired Padding Counts for each perceptron.
/// @param[in] w_opt 
/// Pre-weights.
/// @param Act 
/// Activasion. it will be set globally.
/// @param CalDelta 
/// Delta calculation. It will be set globally.
/// @param[in] outc 
/// Desired output count
/// @param[in] offset_opt 
/// Additional byte allocated
/// @param[out] err_opt
/// State number when needed
/// @return 
/// Its desired size
/// @ref ae2f_mAnnSlpInitSz(outc, offset_opt)
ae2f_extern ae2f_SHAREDCALL
size_t ae2f_mAnnSlpInit(
    ae2f_mAnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt
) noexcept;

/// @memberof ae2f_mAnnSlp
/// @brief 
/// @param[out] _this 
/// Allocated raw vector.
/// @param[in] incs
/// Desired Input Counts for each perceptron.
/// @param[in] inpads_opt 
/// Desired Padding Counts for each perceptron.
/// @param[in] w_opt 
/// Pre-weights.
/// @param Act 
/// Activasion. it will be set globally.
/// @param CalDelta 
/// Delta calculation. It will be set globally.
/// @param[in] outc 
/// Desired output count
/// @param[in] offset_opt 
/// Additional byte allocated
/// @param[out] err_opt
/// State number when needed
/// @return 
/// Its desired size
/// @ref ae2f_mAnnSlpInitSz(outc, offset_opt)
#define ae2f_mAnnSlpInitA(_this, incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_mAnnSlpInit(_this, incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @memberof ae2f_mAnnSlp
/// @brief 
/// @param[out] _this 
/// Allocated raw vector.
/// @param ginc
/// Global Input Count
/// @param[in] inpads_opt 
/// Desired Padding Counts for each perceptron.
/// @param[in] w_opt 
/// Pre-weights.
/// @param Act 
/// Activasion. it will be set globally.
/// @param CalDelta 
/// Delta calculation. It will be set globally.
/// @param[in] outc 
/// Desired output count
/// @param[in] offset_opt 
/// Additional byte allocated
/// @param[out] err_opt
/// State number when needed
/// @return 
/// Its desired size
/// @ref ae2f_mAnnSlpInitSz(outc, offset_opt)
#define ae2f_mAnnSlpInitB(_this, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_mAnnSlpInit(_this, 0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

typedef union ae2f_AnnSlp {
    ae2f_mAnnSlp Slp;
    #if ae2f_WhenCXX(!) 0
    #include "Slp.h.cxx/Slp.hh"
    #endif 
} ae2f_AnnSlp;

/// @memberof ae2f_AnnSlp
/// @brief 
/// Makes a typical( @ref ae2f_mAnnSp::expected ) perceptron.
/// See @ref ae2f_mAnnSlpInit.
/// 
/// It is heap-allocated. pass the output @ref ae2f_mAnnSpDel after use.
ae2f_extern ae2f_SHAREDCALL
ae2f_AnnSlp* ae2f_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt
) noexcept;

/// @memberof ae2f_mAnnSlp
/// @brief 
/// Makes a typical( @ref ae2f_mAnnSp::expected ) perceptron.
/// See @ref ae2f_mAnnSlpInit.
/// 
/// It is heap-allocated. pass the output @ref ae2f_mAnnSpDel after use.
#define ae2f_AnnSlpMkA(incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_AnnSlpMk(incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @memberof ae2f_mAnnSlp
/// @brief 
/// Makes a typical( @ref ae2f_mAnnSp::expected ) perceptron.
/// See @ref ae2f_mAnnSlpInit.
/// 
/// It is heap-allocated. pass the output @ref ae2f_mAnnSpDel after use.
#define ae2f_AnnSlpMkB(ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_AnnSlpMk(0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @fn ae2f_mAnnSlpDel
/// @memberof ae2f_mAnnSlp
/// @param[out] _this 
/// This memory will be freed.
ae2f_AnnDelDef(ae2f_mAnnSlp) noexcept {
    #include "Sp.h.c/clean.script.h"
}

#define ae2f_AnnSlpClean(prm_Slp) ae2f_mAnnSlpClean(&(prm_Slp)->Slp)
#define ae2f_AnnSlpDel(prm_Slp) ae2f_mAnnSlpDel(&(prm_Slp)->Slp)

#if ae2f_WhenCXX(!)0
#include "Slp.h.cxx/imp.hh"
#endif

#include <ae2f/Pack/End.h>

#endif