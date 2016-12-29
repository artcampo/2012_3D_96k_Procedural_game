#include "Geometry\Spline\SplineBezierTimeMapped.hpp"

//===========================================================================
/*!
    Init.

    \param      *aData    Pointer to D3DXVECTOR3 array
    \param      aNumNodes Number of nodes

*/
//===========================================================================

SplineBezierTimeMapped::
  SplineBezierTimeMapped( D3DXVECTOR3* aData, 
                          int aNumNodes, 
                          int aPointsPerArch,
                          float aTime0,
                          float aTimeF,
                          float aDataMultiplier )
    : SplineBezierTMapped (aData, aNumNodes, aPointsPerArch)
{

  mTime0 = aTime0;
  mTimeF = aTimeF;
  mDataMultiplier = aDataMultiplier;
}

SplineBezierTimeMapped::~SplineBezierTimeMapped()
{
  SplineBezierTMapped::~SplineBezierTMapped();
}

D3DXVECTOR3 SplineBezierTimeMapped::getPoint ( float t )
{
  float time = t;
  if ( time < mTime0 ) 
    time = mTime0;
  if ( time > mTimeF ) 
    time = mTimeF;

  float tSpline = ( time - mTime0) / mTimeF;
  D3DXVECTOR3 ret = SplineBezierTMapped::getPoint ( tSpline );
  ret *= mDataMultiplier;
  return ret;
}