#include "Geometry/Interpolation.hpp"

namespace Interpolation{

float bias( const float b, const float x)
{
  //return pow( x, log(b)*(1.0f/(-0.301029995f)) );
  if ( x == 0.0f ) return 0.0f;
  //return pow( x, log(b)*(1.0f/log(2.0f)) );
  return pow( x, log(b)/log(0.5f) );
}

float gain( const float g, const float x)
{
  if ( x < 0.5f ) return bias( 1.0f - g, 2.0f*x)*0.5f;
  else            return 1.0f - bias( 1.0f - g, 2.0f - 2.0f*x)*0.5f;
}

float smootstep( const float x )
{
  return (x*x * (3.0f - 2*x));
}

};
