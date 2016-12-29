#include "LoftBoolean.hpp"
#include "Loft.hpp"
#include "Global.hpp"
#include <algorithm>

namespace LoftBoolean{

  
D3DXVECTOR3 loftPoint( const D3DXVECTOR3& c, 
                      const D3DXMATRIX& f, 
                      const float tp, 
                      const float ts, 
                       SplineBezierTMapped* aSplineSliceBeg, 
                       SplineBezierTMapped* aSplineSliceEnd )
{
  D3DXVECTOR3 p;
  p = aSplineSliceBeg->getPoint( ts )*(1.0f - tp) 
      + aSplineSliceEnd->getPoint( ts )*(tp);
  D3DXVec3TransformCoord ( &p, &p, &f );
  p += c;
  return p;
}


bool loftIntersectSearch(   const D3DXVECTOR3& cA, 
                  const D3DXMATRIX& fA, 
                  const float tpA, 
                  float& tsA, 
                  const D3DXVECTOR3& cB, 
                  const D3DXMATRIX& fB, 
                  const float tpB, 
                  float& tsB, 
                  const LoftInfo& aSurfaceA, 
                  const LoftInfo& aSurfaceB,
                  const bool getToZero)
{
  float epsilon = 1;

  bool found = false;
  float delta = 0.01f;

  while ( !found )
  {
    //  Use any of the advancings
    std::vector<Delta> increments (3);
    {
      float da = delta;
      float db = delta;
      D3DXVECTOR3 pA, pB;
      pA = loftPoint( cA, fA, tpA, tsA + da, aSurfaceA.aSplineSliceBeg, aSurfaceA.aSplineSliceEnd );
      pB = loftPoint( cB, fB, tpB, tsB + db, aSurfaceB.aSplineSliceBeg, aSurfaceB.aSplineSliceEnd );
      float d = DxMath::distance( pA, pB );
      increments[0] =  Delta( da, db, d );
    }
    {
      float da = 0;
      float db = delta;
      D3DXVECTOR3 pA, pB;
      pA = loftPoint( cA, fA, tpA, tsA + da, aSurfaceA.aSplineSliceBeg, aSurfaceA.aSplineSliceEnd );
      pB = loftPoint( cB, fB, tpB, tsB + db, aSurfaceB.aSplineSliceBeg, aSurfaceB.aSplineSliceEnd );
      float d = DxMath::distance( pA, pB );
      increments[1] =  Delta( da, db, d );
    }
    {
      float da = delta;
      float db = 0;
      D3DXVECTOR3 pA, pB;
      pA = loftPoint( cA, fA, tpA, tsA + da, aSurfaceA.aSplineSliceBeg, aSurfaceA.aSplineSliceEnd );
      pB = loftPoint( cB, fB, tpB, tsB + db, aSurfaceB.aSplineSliceBeg, aSurfaceB.aSplineSliceEnd );
      float d = DxMath::distance( pA, pB );
      increments[2] =  Delta( da, db, d );
    }

    std::sort(  increments.begin(), 
                increments.end(), 
                Delta::compare);

    tsA += increments[0].dsa;
    tsB += increments[0].dsb;

    if ( tsA >= 1 || tsB >= 1 )
      return false;

    //  Check end
    if ( getToZero )
      found = ( increments[0].distance < epsilon );
    else
      found = ( increments[0].distance > epsilon );
  }

  return true;
}


//  Assumes:
//    - both splines have same tessellations for path
//    - both splines totally intersect either at start or end
//    - slices are convex polygons    
LoftIntersection  loftIntersect(  const LoftInfo& aSurfaceA, 
                                  const LoftInfo& aSurfaceB,
                                  const bool aInitialTZero)
{
  float tpA, tpB;
  float pathUnitsInc;  
  (aInitialTZero) ? tpA = 0 : tpA = 1;  
  (aInitialTZero) ? tpB = 0 : tpB = 1;  
  (aInitialTZero) ? (pathUnitsInc =  1.0f) : (pathUnitsInc = -1.0f);

  float sliceTInc = 1.0f / float( aSurfaceA.aSliceTes );
  float sliceEpsilonTesellation = 32;
  float sliceEpsilon = sliceTInc / sliceEpsilonTesellation;

  bool  noMoreIntersections = false;

  std::vector< LoftSliceCut >  intersectionsA;
  std::vector< LoftSliceCut >  intersectionsB;

  //while ( !noMoreIntersections )
  for ( int i = 0 ; i < aSurfaceA.aPathTes && !noMoreIntersections; ++i )
  {
    //  Advance to next slice
    aSurfaceA.aSplinePath->advanceAlongPathNunits( pathUnitsInc, tpA, tpA );
    aSurfaceB.aSplinePath->advanceAlongPathNunits( pathUnitsInc, tpB, tpB );
    
    //  Compute point in path and matrices
    D3DXMATRIX  fA, fB;
    D3DXVECTOR3 cA, cB;
    D3DXVECTOR3 pA, pB;

    cA  = aSurfaceA.aSplinePath->getPoint( tpA );
    cB  = aSurfaceB.aSplinePath->getPoint( tpB );

    fA  = Loft::loftBasisForSlice ( aSurfaceA.aSplinePath, 
                                    tpA,
                                    aSurfaceA.aInitialTangent, 
                                    aSurfaceA.aLastTangent, 
                                    aSurfaceA.aInitialNormal, 
                                    aSurfaceA.aLastNormal);

    fB  = Loft::loftBasisForSlice ( aSurfaceB.aSplinePath, 
                                    tpB,
                                    aSurfaceB.aInitialTangent, 
                                    aSurfaceB.aLastTangent, 
                                    aSurfaceB.aInitialNormal, 
                                    aSurfaceB.aLastNormal);

    bool  prev = false;
    float prevTsA, prevTsB;
    bool  twoZeroesFound = false;
    float tsA = 0;
    float tsB = 0;
    float minDist = 1000;


    float tsA_1, tsB_1, tsA_2, tsB_2;

    //  get first zero
    bool  point1_found = loftIntersectSearch( cA, fA, tpA, tsA, cB, fB, tpB, tsB,
                                              aSurfaceA, aSurfaceB, true );  

    tsA_1 = tsA;
    tsB_1 = tsB;
    // go out of first zero
    loftIntersectSearch( cA, fA, tpA, tsA, cB, fB, tpB, tsB,
                                              aSurfaceA, aSurfaceB, false ); 

    //  get second zero
    bool  point2_found = loftIntersectSearch( cA, fA, tpA, tsA, cB, fB, tpB, tsB,
                                              aSurfaceA, aSurfaceB, true );  
    tsA_2 = tsA;
    tsB_2 = tsB;

    // go out of second zero
    loftIntersectSearch( cA, fA, tpA, tsA, cB, fB, tpB, tsB,
                                              aSurfaceA, aSurfaceB, false ); 
    bool  point3_found = loftIntersectSearch( cA, fA, tpA, tsA, cB, fB, tpB, tsB,
                                              aSurfaceA, aSurfaceB, true );  

    
    bool slicesIntersect;
    if ( point1_found && point2_found && !point3_found)
    {
      intersectionsA.push_back( LoftSliceCut( (tsA_1 < tsB_1), tsA_1, tsA_2, tpA ) );
      intersectionsB.push_back( LoftSliceCut( (tsA_1 > tsB_1), tsB_1, tsB_2, tpB ) );
      slicesIntersect = true;
    }

    //  Check intersections
    if ( !slicesIntersect )
      noMoreIntersections = true;
  }

  LoftIntersection intersection;

  intersection.surfaceA.cuts = intersectionsA;
  intersection.surfaceB.cuts = intersectionsB;

  return intersection;
}


LoftIntersection  loftIntersectStart(  const LoftInfo& aSurfaceA, 
                                  const LoftInfo& aSurfaceB )
{
  return loftIntersect( aSurfaceA, aSurfaceB, true );
}

LoftIntersection  loftIntersectEnd(  const LoftInfo& aSurfaceA, 
                                  const LoftInfo& aSurfaceB )
{
  return loftIntersect( aSurfaceA, aSurfaceB, false );
}

};
