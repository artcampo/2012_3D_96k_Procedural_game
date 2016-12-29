#ifndef _LOFT_BOOLEAN_HPP
#define _LOFT_BOOLEAN_HPP

#include "Geometry\Spline\SplineBezierTMapped.hpp"
#include "Geometry\DxMathHelpers.hpp"
#include "dx_misc.hpp"

namespace LoftBoolean{

class LoftInfo{
public:
  LoftInfo(){};
  SplineBezierTMapped*  aSplinePath;
  SplineBezierTMapped*  aSplineSliceBeg;
  SplineBezierTMapped*  aSplineSliceEnd;  
  int aPathTes;
  int aSliceTes;
  D3DXVECTOR3 aSliceMult;
  D3DXVECTOR3 aInitialTangent;
  D3DXVECTOR3 aLastTangent;
  D3DXVECTOR3 aInitialNormal;
  D3DXVECTOR3 aLastNormal;
};

class LoftSliceCut{
public:
  LoftSliceCut( bool reject, float _ts0, float _ts1, float _tp )
  {
    rejectZone = reject;
    ts0 = _ts0;
    ts1 = _ts1;
    tp = _tp;
  };

  bool  rejectZone;   // if true the interval is rejection, otherwise is acceptance
  float ts0;
  float ts1;
  float tp;
};

struct LoftIntersectionInfo{ 
  std::vector<LoftSliceCut> cuts;
};

struct LoftIntersection{
  LoftIntersectionInfo surfaceA;
  LoftIntersectionInfo surfaceB;
};

LoftIntersection  loftIntersectStart(  const LoftInfo& aSurfaceA, 
                                  const LoftInfo& aSurfaceB ); 
LoftIntersection  loftIntersectEnd(  const LoftInfo& aSurfaceA, 
                                  const LoftInfo& aSurfaceB ); 



  class Delta{
  public:
    Delta(){};
    Delta( const float a, const float b, const float d )
    {
      dsa = a;
      dsb = b;
      distance = d;
    };
    float dsa;
    float dsb;
    float distance;
    bool  static compare( const Delta& a, const Delta& b )
    {
      return a.distance < b.distance;
    }
  };

};

#endif // _LOFT_BOOLEAN_HPP
