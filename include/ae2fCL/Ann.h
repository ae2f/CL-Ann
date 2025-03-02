/**
 * @file Ann.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ae2fCL_Ann_h
#define ae2fCL_Ann_h

#include <CL/cl.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>

/// @brief 
/// The list of OpenCL device functions that is able to run after the load of library.
/// 
/// Every element of this enum has a same name as device function. (kernel)
enum ae2fCL_eAnnKerns {
    ae2fCL_eAnnKernsSpMkRand,
    ae2fCL_eAnnKernsSpPredict,
    ae2fCL_eAnnKernsSpTrain,

    /// @brief Count refers to @see ae2fCL_AnnKerns.
    ae2fCL_eAnnKerns_LEN
};

/// @brief 
/// The vector where kernels are located.
ae2f_extern ae2f_SHAREDCALL 
cl_kernel ae2fCL_AnnKerns[
    ae2fCL_eAnnKerns_LEN
];

/// @brief 
/// @param ctx 
/// @param devCount 
/// @param[in] devs  
/// @return 
ae2f_extern ae2f_SHAREDCALL cl_int ae2fCL_AnnMk(
    cl_context ctx,
    cl_uint devCount,
    const cl_device_id* devs
);

/// @brief 
/// @return 
ae2f_extern ae2f_SHAREDCALL cl_int ae2fCL_AnnDel();

#endif