/**
 * @file Act.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2fCL_AnnAct_h
#define ae2fCL_AnnAct_h

#include <ae2f/Float.h>

/// @brief Customisable activasion function pointer type.
typedef ae2f_float_t ae2f_AnnAct_t 
    (ae2f_float_t x);

/// @brief Customisable activasion function pointer type.
typedef ae2f_AnnAct_t* ae2f_fpAnnAct_t;

/// @brief
/// Specify the way of calculating loss.
/// 
/// This will be multiplied with learning rate, \n
/// and that value will be added to each weights field and bias.
/// @param out Predicted value
/// @param goal Expected value (wanted)
/// @return Calculated loss.
typedef ae2f_float_t ae2f_AnnDelta_t (ae2f_float_t out, ae2f_float_t goal);

/// @brief
/// Specify the way of calculating loss.
/// 
/// This will be multiplied with learning rate, \n
/// and that value will be added to each weights field and bias.
/// @param out Predicted value
/// @param goal Expected value (wanted)
/// @return Calculated loss.
typedef ae2f_AnnDelta_t *ae2f_fpAnnDelta_t;

#endif