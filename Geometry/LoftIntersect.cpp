#include "LoftIntersect.hpp"
#include "Geometry\Loft.hpp"

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
                  std::vector<VertPNTUV>& aVertices )
{  

  float pathTInc  = 1.0f / float( aPathTes );
  float sliceTInc = 1.0f / float( aSliceTes );
  float tp = 0.0f;
  float pathAccLength = 0;

  float tpEnd = 1.0f;
  if ( aHasIntersectionStart )
    tp = aIntStart.cuts[ aIntStart.cuts.size() - 1].tp;
  if ( aHasIntersectionEnd )
    tpEnd = aIntEnd.cuts[0].tp;

  //  Final number of vertices is unknow  
  bool endReached = false;
  while (!endReached)
  {

    float ts        = 0.0f;  
    float sliceTMax = 1.0f;

    bool  reject;
    float reject_ts1;
    float reject_ts2;
    bool found = false;
    
    if ( aHasIntersectionStart )
    {      
      for ( int i = 0; i < aIntStart.cuts.size() && ! found ; ++i )
      {
        found = DxMath::equality( tp, aIntStart.cuts[i].tp, 0.01f );
        if ( found )
        {
          reject      = aIntStart.cuts[i].rejectZone;
          reject_ts1  = aIntStart.cuts[i].ts0;
          reject_ts2  = aIntStart.cuts[i].ts1;
        }
      }

    }
    if ( aHasIntersectionEnd )
    {      
      for ( int i = 0; i < aIntEnd.cuts.size() && ! found ; ++i )
      {
        found = DxMath::equality( tp, aIntEnd.cuts[i].tp, 0.01f );
        if ( found )
        {
          reject      = aIntStart.cuts[i].rejectZone;
          reject_ts1  = aIntStart.cuts[i].ts0;
          reject_ts2  = aIntStart.cuts[i].ts1;
        }
      }
    }

    float currentPathLength;
    float currentSliceLength;
    float sliceAccLength = 0;
    for ( int j = 0 ; j < aSliceTes ; ++j, ts += sliceTInc )    
    {
      float componentU = 1;
      if ( found )
      {
        if ( reject )
        {
          if ( ts >= reject_ts1 && ts <= reject_ts2 );
            //continue;
            componentU = 0;
        }
        else
        {
          if ( ts <= reject_ts1 && ts >= reject_ts2 );
            //continue;
            componentU = 0;
        }

      }
      D3DXVECTOR3 p00, p01, p10, p11;
      D3DXMATRIX  f0, f1;
      D3DXVECTOR3 c0, c1;

      c0  = aSplinePath->getPoint( tp );
      c1  = aSplinePath->getPoint( tp + pathTInc );      

      currentPathLength   = DxMath::distance( c1, c0 );
      D3DXVECTOR3 s0, s1; 
      s0 = aSplineSliceBeg->getPoint( ts )*(1.0f - tp) + aSplineSliceEnd->getPoint( ts )*(tp);
      s1 = aSplineSliceBeg->getPoint( ts + sliceTInc )*(1.0f - tp) + aSplineSliceEnd->getPoint( ts + sliceTInc )*(tp);
      DxMath::componentMult( s0 , aSliceMult );
      DxMath::componentMult( s1 , aSliceMult );
      currentSliceLength  = DxMath::distance( s0 , s1 );    

      p00 = aSplineSliceBeg->getPoint( ts )*(1.0f - tp) 
          + aSplineSliceEnd->getPoint( ts )*(tp);
      p10 = aSplineSliceBeg->getPoint( ts )*(1.0f - tp)
          + aSplineSliceEnd->getPoint( ts )*(tp);
      p01 = aSplineSliceBeg->getPoint( ts + sliceTInc )*(1.0f - tp)
          + aSplineSliceEnd->getPoint( ts + sliceTInc )*(tp);
      p11 = aSplineSliceBeg->getPoint( ts + sliceTInc )*(1.0f - tp)
          + aSplineSliceEnd->getPoint( ts + sliceTInc )*(tp);
     

      D3DXVECTOR2 uv00, uv01, uv10, uv11;
      /*
      uv00 = D3DXVECTOR2( tp, ts );
      uv10 = D3DXVECTOR2( tp + pathTInc, ts );
      uv01 = D3DXVECTOR2( tp, ts + sliceTInc);
      uv11 = D3DXVECTOR2( tp + pathTInc, ts + sliceTInc);

*/
      uv00 = D3DXVECTOR2( pathAccLength, sliceAccLength  );
      uv10 = D3DXVECTOR2( pathAccLength + currentPathLength, sliceAccLength);
      uv01 = D3DXVECTOR2( pathAccLength, sliceAccLength + currentSliceLength);
      uv11 = D3DXVECTOR2( pathAccLength + currentPathLength, sliceAccLength + currentSliceLength);

      DxMath::componentMult( p00, aSliceMult );
      DxMath::componentMult( p01, aSliceMult );
      DxMath::componentMult( p10, aSliceMult );
      DxMath::componentMult( p11, aSliceMult );

      f0  = loftBasisForSlice ( aSplinePath, tp,
                          aInitialTangent, aLastTangent, aInitialNormal, aLastNormal);
      f1  = loftBasisForSlice ( aSplinePath, tp + pathTInc,
                          aInitialTangent, aLastTangent, aInitialNormal, aLastNormal);

      D3DXVec3TransformCoord ( &p00, &p00, &f0 );
      D3DXVec3TransformCoord ( &p01, &p01, &f0 );
      D3DXVec3TransformCoord ( &p10, &p10, &f1 );
      D3DXVec3TransformCoord ( &p11, &p11, &f1 );

      p00 += c0;
      p01 += c0;
      p10 += c1;
      p11 += c1;

      //triangle 1
      VertPNTUV v;   

      v.tan = tangentBezier( aSplinePath, tp,
                            aInitialTangent, aLastTangent, aInitialNormal, aLastNormal);

      v.nor = DxMath::normalOfTriangle( p10, p11, p01 );
/*
      //triangle 1  
      v.pos = p00;  v.uv = uv00;  aVertices.push_back( v );
      v.pos = p01;  v.uv = uv01;  aVertices.push_back( v);
      v.pos = p10;  v.uv = uv10;  aVertices.push_back( v);
     
      //triangle 2     
      v.pos = p01;  v.uv = uv01;  aVertices.push_back( v);
      v.pos = p11;  v.uv = uv11;  aVertices.push_back( v);
      v.pos = p10;  v.uv = uv10;  aVertices.push_back( v);
*/
      
      v.uv = D3DXVECTOR2( componentU, 0 );
      //triangle 1  
      v.pos = p00;   aVertices.push_back( v );
      v.pos = p01;   aVertices.push_back( v);
      v.pos = p10;  aVertices.push_back( v);
     
      //triangle 2     
      v.pos = p01; aVertices.push_back( v);
      v.pos = p11;  aVertices.push_back( v);
      v.pos = p10;   aVertices.push_back( v);

      sliceAccLength += currentSliceLength;
    } //  end of slice process
  
    pathAccLength += currentPathLength;

    //  Advance tp
    endReached = !aSplinePath->advanceAlongPathNunits( 1, tp, tp );

    if ( tp >= tpEnd )
      endReached = true;
  }

}

};
