#include "Loft.hpp"
#include "Global.hpp"

#include "Project_Dependant/ConditionalCompilation.hpp"
#include "Geometry/Interpolation.hpp"

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
                  std::vector<VertPNTUV>& aVertices )
{  

  float pathTInc  = 1.0f / float( aPathTes );
  float sliceTInc = 1.0f / float( aSliceTes );
  float tp = 0.0f;
  float pathAccLength = 0;

#ifdef _DP_LOFT_DEBUG_LOG
  Global::mFileDebugTrack->precision(3);
  *Global::mFileDebugTrack << "------------------------------------------------------------" << std::endl;
  *Global::mFileDebugTrack << "loft "<< std::endl;
        
        *Global::mFileDebugTrack << "Ini_tan:"<< aInitialTangent.x << "," <<aInitialTangent.y<<","<<aInitialTangent.z<<std::endl;        
        *Global::mFileDebugTrack << "End_tan:"<< aLastTangent.x << "," <<aLastTangent.y<<","<<aLastTangent.z<<std::endl;
        *Global::mFileDebugTrack << "Ini_nor:"<< aInitialNormal.x << "," <<aInitialNormal.y<<","<<aInitialNormal.z<<std::endl;        
        *Global::mFileDebugTrack << "End_nor:"<< aLastNormal.x << "," <<aLastNormal.y<<","<<aLastNormal.z<<std::endl;
  *Global::mFileDebugTrack << "------------------------------------------------------------" << std::endl;
#endif

  //  Final number of vertices is:
  //  aPathTes*aSliceTes*6
  aVertices.resize( aPathTes*aSliceTes*6 );
  int verticesIndex = 0;
  for ( int i = 0 ; i < aPathTes ; ++i, tp += pathTInc )
  {
#ifdef _DP_LOFT_DEBUG_LOG
    {
      D3DXVECTOR3 tangent = tangentBezier( aSplinePath, tp, 
        aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
      D3DXVECTOR3 normal  = normalBezier( aSplinePath, tp, 
        aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
      //DxMath::forcePerpendicularity( tangent, normal );
      
      Global::mFileDebugTrack->precision( 3 );

      Global::mFileDebugTrack->precision( 8 );
      float dot = D3DXVec3Dot( &tangent, &normal );
      *Global::mFileDebugTrack << "dot:"<<dot<<std::endl;

    }
#endif

    float ts = 0.0f;    

    /*
    float renormalizationOfEndings = 0.15f;
    float tpRen = (tp - (renormalizationOfEndings*2.0f)) / (1.0f - (renormalizationOfEndings*2.0f));
    if ( tp < renormalizationOfEndings )
      tpRen = 0.0f;
    if ( tp > (1.0f - renormalizationOfEndings) )
      tpRen = 1.0f;
    */
    /*
    float tpRen  = Interpolation::gain(0.7f, tp);
    float tpRen2 = Interpolation::gain(0.7f, tp + pathTInc);
    
    float tpRen  = tp;
    float tpRen2 = tp + pathTInc;
*/
    
    float tpRen  = Interpolation::smootstep( tp);
    float tpRen2 = Interpolation::smootstep( tp + pathTInc);
    



    float currentPathLength;    
    float sliceAccLengthCurrent = 0;
    float sliceAccLengthNext = 0;

    for ( int j = 0 ; j < aSliceTes ; ++j, ts += sliceTInc )
    {
      D3DXVECTOR3 p00, p01, p10, p11;
      D3DXMATRIX  f0, f1;
      D3DXVECTOR3 c0, c1;

      c0  = aSplinePath->getPoint( tp );
      c1  = aSplinePath->getPoint( tp + pathTInc );

      currentPathLength   = DxMath::distance( c1, c0 );

      D3DXVECTOR3 s0a, s1a; 
      D3DXVECTOR3 s0b, s1b; 


      p00 = aSplineSliceBeg->getPoint( ts )*(1.0f - tpRen) 
          + aSplineSliceEnd->getPoint( ts )*(tpRen);
      p10 = aSplineSliceBeg->getPoint( ts )*(1.0f - tpRen2)
          + aSplineSliceEnd->getPoint( ts )*(tpRen2);
      p01 = aSplineSliceBeg->getPoint( ts + sliceTInc )*(1.0f - tpRen)
          + aSplineSliceEnd->getPoint( ts + sliceTInc )*(tpRen);
      p11 = aSplineSliceBeg->getPoint( ts + sliceTInc )*(1.0f - tpRen2)
          + aSplineSliceEnd->getPoint( ts + sliceTInc )*(tpRen2);

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

      float sliceLengthCurrent  = DxMath::distance( p00, p01 );
      float sliceLengthNext     = DxMath::distance( p10, p11 );

      D3DXVECTOR2 uv00, uv01, uv10, uv11;      
      uv00 = D3DXVECTOR2( sliceAccLengthCurrent,                      pathAccLength );
      uv10 = D3DXVECTOR2( sliceAccLengthNext,                         pathAccLength + currentPathLength);
      uv01 = D3DXVECTOR2( sliceAccLengthCurrent + sliceLengthCurrent, pathAccLength);
      uv11 = D3DXVECTOR2( sliceAccLengthNext + sliceLengthNext,       pathAccLength + currentPathLength);


      p00 += c0;
      p01 += c0;
      p10 += c1;
      p11 += c1;

      //triangle 1
      VertPNTUV v;   

      v.tan = tangentBezier( aSplinePath, tp,
                            aInitialTangent, aLastTangent, aInitialNormal, aLastNormal);

      v.nor = DxMath::normalOfTriangle( p10, p11, p01 );

      //triangle 1      
      v.pos = p00;  v.uv = uv00;  aVertices[verticesIndex + 0] = v;
      v.pos = p01;  v.uv = uv01;  aVertices[verticesIndex + 1] = v;
      v.pos = p10;  v.uv = uv10;  aVertices[verticesIndex + 2] = v;
     
      //triangle 2     
      v.pos = p01;  v.uv = uv01;  aVertices[verticesIndex + 3] = v;
      v.pos = p11;  v.uv = uv11;  aVertices[verticesIndex + 4] = v;
      v.pos = p10;  v.uv = uv10;  aVertices[verticesIndex + 5] = v;
      verticesIndex += 6;    

      sliceAccLengthCurrent += sliceLengthCurrent;
      sliceAccLengthNext    += sliceLengthNext;
    } //  end of slice process

    pathAccLength += currentPathLength;
  }

  //  print last t
#ifdef _DP_LOFT_DEBUG_LOG
    {
      D3DXVECTOR3 tangent = tangentBezier( aSplinePath, tp, 
        aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
      D3DXVECTOR3 normal  = normalBezier( aSplinePath, tp, 
        aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
      
     
      Global::mFileDebugTrack->precision( 8 );
      float dot = D3DXVec3Dot( &tangent, &normal );
      *Global::mFileDebugTrack << "dot:"<<dot<<std::endl;
    }
#endif

}




DxMath::TnbBasis  basisBezier( SplineBezierTMapped* aSpline, 
                        float aT,
                        const D3DXVECTOR3& aInitialTangent, 
                        const D3DXVECTOR3& aLastTangent, 
                        const D3DXVECTOR3& aInitialNormal,
                        const D3DXVECTOR3& aLastNormal)
{
 
  D3DXVECTOR3 tangent = tangentBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
  D3DXVECTOR3 normal  = normalBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
  
  D3DXVECTOR3 i = tangent;
  D3DXVECTOR3 j = normal;  
  D3DXVECTOR3 k = DxMath::thirdVectorBasis( tangent, normal );
  
  DxMath::TnbBasis b;
  b.tan = i;
  b.nor = j;
  b.bin = k;

  return b;  
}


D3DXMATRIX  loftBasisForSlice( SplineBezierTMapped* aSpline, 
                        float aT,
                        const D3DXVECTOR3& aInitialTangent, 
                        const D3DXVECTOR3& aLastTangent, 
                        const D3DXVECTOR3& aInitialNormal,
                        const D3DXVECTOR3& aLastNormal)
{ 
  D3DXVECTOR3 tangent = tangentBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
  D3DXVECTOR3 normal  = normalBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );  
  D3DXVECTOR3 bitangent  = bitangentBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );  

  return DxMath::basisMatrixInv( tangent, normal, bitangent );  
}


D3DXVECTOR3  tangentBezier( SplineBezierTMapped* aSpline, float aT,
                          const D3DXVECTOR3& aInitialTangent, 
                          const D3DXVECTOR3& aLastTangent, 
                          const D3DXVECTOR3& aInitialNormal,
                          const D3DXVECTOR3& aLastNormal)
{
  if ( DxMath::equality( aT, 0, 0.01f )) return aInitialTangent;
  if ( DxMath::equality( aT, 1, 0.01f )) return aLastTangent;

  D3DXVECTOR3 tangent    = aSpline->getPointTangent( aT );   
  D3DXVec3Normalize ( &tangent, &tangent );

  return tangent;
}

D3DXVECTOR3  bitangentBezier( SplineBezierTMapped* aSpline, float aT,
                          const D3DXVECTOR3& aInitialTangent, 
                          const D3DXVECTOR3& aLastTangent, 
                          const D3DXVECTOR3& aInitialNormal,
                          const D3DXVECTOR3& aLastNormal)
{
  D3DXVECTOR3 tangent = tangentBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
  D3DXVECTOR3 normal  = normalBezier( aSpline, aT, 
    aInitialTangent, aLastTangent, aInitialNormal, aLastNormal );
  
  D3DXVECTOR3 i = tangent;
  D3DXVECTOR3 j = normal;  
    
  return DxMath::thirdVectorBasis( tangent, normal );
}


D3DXVECTOR3  normalBezier( SplineBezierTMapped* aSpline, float aT,
                          const D3DXVECTOR3& aInitialTangent, 
                          const D3DXVECTOR3& aLastTangent, 
                          const D3DXVECTOR3& aInitialNormal,
                          const D3DXVECTOR3& aLastNormal)
{
  D3DXVECTOR3 tangent = tangentBezier(  aSpline, aT, 
                                        aInitialTangent, aLastTangent,
                                        aInitialNormal, aLastNormal );

  

  D3DXVECTOR3 lastNormal = DxMath::alignToTangents( tangent, aLastTangent, aLastNormal );  
  D3DXVECTOR3 initialNormal = DxMath::alignToTangents( tangent, aInitialTangent, aInitialNormal );  

  D3DXVECTOR3 normal        = lastNormal*(aT)  + initialNormal*(1.0f - aT);  
  D3DXVec3Normalize ( &normal, &normal );
  

  return normal;
}

//  Makes a piece of an spline with two nodes
std::vector<D3DXVECTOR3>  splinePath( const D3DXVECTOR3& aPointA, 
                                      const D3DXVECTOR3& aTanA, 
                                      const D3DXVECTOR3& aPointB, 
                                      const D3DXVECTOR3& aTanB,
                                      const float aLengthTan )
{
  std::vector<D3DXVECTOR3> path (4);  
  path[0] = aPointA;
  path[1] = aPointA + aLengthTan*aTanA;  
  path[2] = aPointB - aLengthTan*aTanB;
  path[3] = aPointB;
  return path;
}

};
