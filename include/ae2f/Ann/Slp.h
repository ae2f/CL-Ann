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

struct ae2f_AnnSlp;

/// @brief cleaning function
typedef ae2f_err_t ae2f_AnnSlpClean_t (ae2f_struct ae2f_AnnSlp*) noexcept;

/// @brief 
/// Predict one output from multiple inputs.
/// @param[in] in 
/// Input vector. \n
/// Its count is @ref ae2f_AnnSp::inc.
/// @param[out] outret_opt 
/// That one predicted. \n
/// Its count is @ref ae2f_AnnSp::inc.
/// @return  State
typedef ae2f_err_t ae2f_AnnSlpPredict_t (
    const ae2f_struct ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* outret_opt
) noexcept;

/// @brief 
/// Training function, multiple input from multiple output.
/// @param[in] in 
/// Input vector. \n
/// Its count is @ref ae2f_AnnSp::inc.
/// @param[in] delta_optA 
/// Delta vector when you pre-calculated it.
/// Its count is one.
/// @param[in] goal_optB 
/// Desired output.
/// @param learningrate 
/// Learning rate
typedef ae2f_err_t ae2f_AnnSlpTrain_t (
    ae2f_struct ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept;

/// @brief 
/// The element type for @ref ae2f_AnnSlp.
typedef struct ae2f_AnnSp ae2f_AnnSlpEl;

/// @brief 
/// # Single Layered Perceptron
/// 
/// Multiple input, multiple output. \n
/// For predicting & training operations are able to be parallel.
typedef struct ae2f_AnnSlp {

    /// @brief 
    /// True when its shape is same as expected. \n
    /// Expected structure is generated by @ref ae2f_AnnSpInit.
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
    ae2f_AnnSlpClean_t* vClean;

    /// @brief
    /// Predict function
    ae2f_AnnSlpPredict_t* vPredict;

    /// @brief
    /// Training function
    ae2f_AnnSlpTrain_t* vTrain;

    #if ae2f_WhenCXX(!)0
    #include "Slp.h.cxx/Slp.hh"
    #endif
} ae2f_AnnSlp;

/// @memberof ae2f_AnnSlp
/// @brief
/// Input padding for perceptron.
/// 
/// Valid when @ref ae2f_AnnSp::expected is 1. \n
/// When not, it is null.
#define ae2f_AnnSlpPerVPad(slp, ...) \
(ae2f_CmpGetMem(slp, expected, 0) ? \
ae2f_reinterpret_cast(__VA_ARGS__ size_t* __VA_ARGS__ *, ae2f_static_cast(__VA_ARGS__ ae2f_AnnSlp*, slp) + 1) : \
0)

/// @memberof ae2f_AnnSlp
/// @brief
/// Perceptron vector. \n
/// Its length is same as @ref ae2f_AnnSlp::outc.
///
/// Valid when @ref ae2f_AnnSlp::expected is 1. \n
/// When not, it is null.
#define ae2f_AnnSlpPerV(slp, i, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ ae2f_AnnSlpEl*, (ae2f_AnnSlpPerVPad(slp, __VA_ARGS__)[i] + 1))

/// @memberof ae2f_AnnSlp
/// @brief
/// Additional buffer allocated.
#define ae2f_AnnSlpX(slp, type, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ type, ae2f_CmpGetMem(slp, expected, 0) ? (ae2f_AnnSlpPerVPad(slp, __VA_ARGS__) + (slp)->layerc) : 0)

/// @memberof ae2f_AnnSlp
/// @brief
/// Desired least byte size for initialising.
#define ae2f_AnnSlpInitSz(outc, off) ((off) + sizeof(ae2f_AnnSlp) + (outc) * sizeof(void*))

/// @memberof ae2f_AnnSlp
/// @brief
/// Caller of @ref ae2f_AnnSlpPredict_t.
#define ae2f_AnnSlpPredict ae2f_AnnSpPredict

/// @memberof ae2f_AnnSlp
/// @brief
/// Caller of @ref ae2f_AnnSlpTrain_t.
#define ae2f_AnnSlpTrain ae2f_AnnSpTrain

/// @memberof ae2f_AnnSlp
/// @brief 
/// Caller of @ref ae2f_AnnSlpTrain_t \n
/// Train with Pre-calculated delta.
#define ae2f_AnnSlpTrainA ae2f_AnnSpTrainA

/// @memberof ae2f_AnnSlp
/// @brief 
/// Caller of @ref ae2f_AnnSlpTrain_t \n
/// Train for desired output: goal.
#define ae2f_AnnSlpTrainB ae2f_AnnSpTrainB

/// @memberof ae2f_AnnSlp
/// @brief Cleaning function
#define ae2f_AnnSlpClean ae2f_AnnSpClean

/// @memberof ae2f_AnnSlp
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
/// @ref ae2f_AnnSlpInitSz(outc, offset_opt)
ae2f_extern ae2f_SHAREDCALL
size_t ae2f_AnnSlpInit(
    ae2f_AnnSlp* _this,
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

/// @memberof ae2f_AnnSlp
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
/// @ref ae2f_AnnSlpInitSz(outc, offset_opt)
#define ae2f_AnnSlpInitA(_this, incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_AnnSlpInit(_this, incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @memberof ae2f_AnnSlp
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
/// @ref ae2f_AnnSlpInitSz(outc, offset_opt)
#define ae2f_AnnSlpInitB(_this, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_AnnSlpInit(_this, 0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @memberof ae2f_AnnSlp
/// @brief 
/// Makes a typical( @ref ae2f_AnnSp::expected ) perceptron.
/// See @ref ae2f_AnnSlpInit.
/// 
/// It is heap-allocated. pass the output @ref ae2f_AnnSpDel after use.
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

/// @memberof ae2f_AnnSlp
/// @brief 
/// Makes a typical( @ref ae2f_AnnSp::expected ) perceptron.
/// See @ref ae2f_AnnSlpInit.
/// 
/// It is heap-allocated. pass the output @ref ae2f_AnnSpDel after use.
#define ae2f_AnnSlpMkA(incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_AnnSlpMk(incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @memberof ae2f_AnnSlp
/// @brief 
/// Makes a typical( @ref ae2f_AnnSp::expected ) perceptron.
/// See @ref ae2f_AnnSlpInit.
/// 
/// It is heap-allocated. pass the output @ref ae2f_AnnSpDel after use.
#define ae2f_AnnSlpMkB(ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt) \
ae2f_AnnSlpMk(0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt)

/// @fn ae2f_AnnSlpDel
/// @memberof ae2f_AnnSlp
/// @param[out] _this 
/// This memory will be freed.
ae2f_AnnDelDef(ae2f_AnnSlp) noexcept {
    #include "Sp.h.c/clean.script.h"
}

#if ae2f_WhenCXX(!)0
#include "Slp.h.cxx/imp.hh"
#endif

#endif