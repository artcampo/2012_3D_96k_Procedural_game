#ifndef _INTERPOLATION_HPP
#define _INTERPOLATION_HPP


//#include "Geometry\DxMathHelpers.hpp"
#include "dx_misc.hpp"

namespace Interpolation{
 
  float bias( const float b, const float x);
  float gain( const float g, const float x);
  float gain( const float g, const float x);
  float smootstep( const float x );
};

#endif // _INTERPOLATION_HPP
