#ifndef _SPLINE_INTEGRAL_HPP
#define _SPLINE_INTEGRAL_HPP

// TODO: mixed class! offers two functionalities!!

#include  <vector>
#include "Geometry/DxMathHelpers.hpp"
#include "Geometry/Spline/SplineBezierTMapped.hpp"

namespace Spline{
float integrateLength( SplineBezierTMapped* aSpline, const int aNumSteps );
};

#endif  //_SPLINE_INTEGRAL_HPP
