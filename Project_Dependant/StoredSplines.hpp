#ifndef _STOREDSPLINES_HPP_
#define _STOREDSPLINES_HPP_

/*!
 * \file SotredSSplines.hpp
 * \brief Static data with splines arrays
 * \author Arturo
 */

#include "dx_misc.hpp"

struct StoredSplineDesc{
    float *data;          
    int   numElem;
};

extern StoredSplineDesc gShipTForceFwdSpline;

extern D3DXVECTOR3 Spline_SliceBorder [];
extern D3DXVECTOR3 Spline_SliceNoBorder [];

namespace StoredSplines {
  static const int gSplineSliceNumElems = 22;
};

#endif