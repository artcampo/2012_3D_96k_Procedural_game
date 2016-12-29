#include "Geometry/Spline/SplineIntegral.hpp"

namespace Spline{


float integrateLength( SplineBezierTMapped* aSpline, const int aNumSteps )
{ 
  float length = 0.0f;

  for ( int i = 0; i < aNumSteps ; ++i )
  {
    float t0 = float(i + 0) / float(aNumSteps);
    float t1 = float(i + 1) / float(aNumSteps);

    D3DXVECTOR3 p0 = aSpline->getPoint( t0 );
    D3DXVECTOR3 p1 = aSpline->getPoint( t1 );

    length += DxMath::distance( p0, p1 );
  }

  return length;
}

};