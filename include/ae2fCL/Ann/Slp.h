/**
 * @file Slp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 * Slp definition
 * 
 */

#ifndef ae2fCL_Ann_Slp_h
#define ae2fCL_Ann_Slp_h

#include <ae2f/Ann/Slp.h>
#include <CL/cl.h>
#include <ae2f/Pack/Beg.h>

typedef struct ae2fCL_mAnnSlpMemX {
	cl_mem In;

	/** @brief Has weights been changed */
	bool Changed;
} ae2fCL_mAnnSlpMemX;

#if ae2f_WhenCXX(!) 0 
struct ae2fCL_mAnnSlp {
#include "Slp.h.cxx/mSlp.hh" 
};
#else

/**
 * @class ae2fCL_mAnnSlp
 * @brief 
 * SLP with OpenCL acceleration.
 */
typedef ae2f_mAnnSlp ae2fCL_mAnnSlp;

#endif

ae2f_extern ae2f_SHAREDCALL
size_t ae2fCL_mAnnSlpInit(
    ae2fCL_mAnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept;

#define ae2fCL_mAnnSlpInitA(_this, incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_mAnnSlpInit(_this, incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

#define ae2fCL_mAnnSlpInitB(_this, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_mAnnSlpInit(_this, 0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

typedef union ae2fCL_AnnSlp {
    ae2f_mAnnSlp Slp;
    ae2fCL_mAnnSlp CL_Slp;

    #if ae2f_WhenCXX(!)0
    #include "Slp.h.cxx/Slp.hh"
    #endif
} ae2fCL_AnnSlp;

ae2f_extern ae2f_SHAREDCALL
ae2fCL_AnnSlp* ae2fCL_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept;

#define ae2fCL_AnnSlpClean ae2f_AnnSlpClean 
#define ae2fCL_AnnSlpDel ae2f_AnnSlpDel 

#define ae2fCL_AnnSlpMkA(incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_AnnSlpMk(incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

#define ae2fCL_AnnSlpMkB(ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_AnnSlpMk(0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

/** 
 * @param outc
 * @param off 
 * */
#define ae2fCL_mAnnSlpInitSz(outc, off) \
	ae2f_mAnnSlpInitSz(outc, (off) + sizeof(ae2fCL_mAnnSlpMemX) + (sizeof(ae2f_float_t) * outc))


#define ae2fCL_mAnnSlpAdd(slp, ...) \
	ae2f_mAnnSlpX(slp, ae2fCL_mAnnSlpMemX*, __VA_ARGS__)

#define ae2fCL_mAnnSlpOutCache(slp, ...) \
	ae2f_reinterpret_cast(__VA_ARGS__ ae2f_float_t*, ae2fCL_mAnnSlpAdd(slp, __VA_ARGS__) + 1)

#define ae2fCL_mAnnSlpX(slp, type, ...) \
 	ae2f_reinterpret_cast(__VA_ARGS__ type*, ae2fCL_mAnnSlpOutCache(slp, __VA_ARGS__) + (slp)->outc)

#include <ae2f/Pack/End.h>

#if ae2f_WhenCXX(!)0
#include "Slp.h.cxx/imp.hh"
#endif

#endif
