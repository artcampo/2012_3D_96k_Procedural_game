#ifndef _SPLINEBEZIERTIMEMAPPED_HPP
#define _SPLINEBEZIERTIMEMAPPED_HPP

#include  <vector>

#include  "dx_misc.hpp"
#include  "Project_Dependant/StoredSplines.hpp"
#include  "Geometry\Spline\SplineBezierTMapped.hpp"

class SplineBezierTimeMapped : private SplineBezierTMapped
{
public:


  SplineBezierTimeMapped( D3DXVECTOR3* aData, 
                          int aNumNodes, 
                          int aPointsPerArch,
                          float aTime0,
                          float aTimeF,
                          float aDataMultiplier );
	~SplineBezierTimeMapped();

  D3DXVECTOR3 getPoint ( float t );

private:
  float mTime0;
  float mTimeF;
  float mDataMultiplier;

};


#endif 
