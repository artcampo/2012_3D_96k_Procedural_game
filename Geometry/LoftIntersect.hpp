#ifndef _LOFT_INTERSECTIONS_HPP
#define _LOFT_INTERSECTIONS_HPP

#include "Geometry\Spline\SplineBezierTMapped.hpp"
#include "Geometry\DxMathHelpers.hpp"
#include "dx_misc.hpp"
#include "Geometry\LoftBoolean.hpp"



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
                    const LoftBoolean::LoftIntersectionInfo& aIntStart, 
                    const LoftBoolean::LoftIntersectionInfo& aIntEnd,
                    const bool aHasIntersectionStart,
                    const bool aHasIntersectionEnd,
                    std::vector<VertPNTUV>& aVertices
                     );

    

};

#endif // _LOFT_INTERSECTIONS_HPP
