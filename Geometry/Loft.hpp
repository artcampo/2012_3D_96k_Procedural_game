#ifndef _LOFT_HPP
#define _LOFT_HPP

#include "Geometry\Spline\SplineBezierTMapped.hpp"
#include "Geometry\DxMathHelpers.hpp"
#include "dx_misc.hpp"
#include "RenderModels\Compounded\AxisModel.hpp"
#include "Geometry\DxMathHelpers.hpp"

namespace Loft{

  void  loftBezier( SplineBezierTMapped*  aSplinePath,
                    SplineBezierTMapped*  aSplineSliceBeg,
                    SplineBezierTMapped*  aSplineSliceEnd,
                    int aPathTes,
                    int aSliceTes,
                    D3DXVECTOR3 aSliceMult,
                    const D3DXVECTOR3& aInitialTangent, 
                    const D3DXVECTOR3& aLastTangent, 
                    const D3DXVECTOR3& aInitialNormal,
                    const D3DXVECTOR3& aLastNormal,
                    std::vector<VertPNTUV>& aVertices);

  
  D3DXMATRIX  loftBasisForSlice( SplineBezierTMapped* aSpline, float aT,
                            const D3DXVECTOR3& aInitialTangent, 
                            const D3DXVECTOR3& aLastTangent, 
                            const D3DXVECTOR3& aInitialNormal,
                            const D3DXVECTOR3& aLastNormal);

  DxMath::TnbBasis    basisBezier( SplineBezierTMapped* aSpline, float aT,
                            const D3DXVECTOR3& aInitialTangent, 
                            const D3DXVECTOR3& aLastTangent, 
                            const D3DXVECTOR3& aInitialNormal,
                            const D3DXVECTOR3& aLastNormal); 

  D3DXVECTOR3  normalBezier( SplineBezierTMapped* aSpline, float aT,
                            const D3DXVECTOR3& aInitialTangent, 
                            const D3DXVECTOR3& aLastTangent, 
                            const D3DXVECTOR3& aInitialNormal,
                            const D3DXVECTOR3& aLastNormal);
  D3DXVECTOR3  tangentBezier( SplineBezierTMapped* aSpline, float aT,
                            const D3DXVECTOR3& aInitialTangent, 
                            const D3DXVECTOR3& aLastTangent, 
                            const D3DXVECTOR3& aInitialNormal,
                            const D3DXVECTOR3& aLastNormal);
  D3DXVECTOR3  bitangentBezier( SplineBezierTMapped* aSpline, float aT,
                            const D3DXVECTOR3& aInitialTangent, 
                            const D3DXVECTOR3& aLastTangent, 
                            const D3DXVECTOR3& aInitialNormal,
                            const D3DXVECTOR3& aLastNormal);

  std::vector<D3DXVECTOR3>  splinePath( const D3DXVECTOR3& aPointA, 
                                        const D3DXVECTOR3& aTanA, 
                                        const D3DXVECTOR3& aPointB, 
                                        const D3DXVECTOR3& aTanB,
                                        const float aLengthTan ); 

};

#endif // _LOFT_HPP
