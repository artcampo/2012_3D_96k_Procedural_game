#include  "GameBall/Track.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesJumps.hpp"

TrackPath* Track::generatePathRamp( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  using namespace StoredPiecesJumps::Ramp;

  TrackBp::Vertex nodeBeg = mGraph.vertex( aEdge.vb );
  TrackBp::Vertex nodeEnd = mGraph.vertex( aEdge.ve );

  float length          = DxMath::distance( nodeEnd.pos, nodeBeg.pos );  
  float lengthTangents  = length*0.1f;
  
    
  D3DXVECTOR3 jumpTangent = (nodeBeg.tan + nodeBeg.nor) * 0.5f;
  D3DXVec3Normalize( &jumpTangent, &jumpTangent );
  D3DXVECTOR3 jumpNormal  = DxMath::alignToTangents( jumpTangent,nodeBeg.tan, nodeBeg.nor );

  TrackBp::Vertex jumpPoint(  nodeBeg.pos + nodeBeg.tan*jumpLength + nodeBeg.nor*jumpHeight,
                              jumpTangent,
                              jumpNormal );

  TrackBp::Vertex landingPoint( nodeBeg.pos + nodeBeg.tan*landLength,
                                nodeBeg.tan,
                                nodeBeg.nor );
                                

  TrackPath* trackPath = new TrackPath(aTrackPathDebugId);

  //  Ramp
  trackPath->addEdge( nodeBeg, jumpPoint, lengthTangents, mSliceBorder, mSliceNoBorder, aTrackWidth, mSliceMultiplier);

  //  Landing  
  trackPath->addEdge( landingPoint, nodeEnd, lengthTangents, mSliceNoBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );

  return trackPath;
}



TrackPath* Track::generatePathNineTurn( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{  
 
  TrackPath* trackPath = new TrackPath(aTrackPathDebugId);  

  return trackPath;
}

TrackPath* Track::generatePathOne80Turn( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  return NULL;
}

TrackPath* Track::generatePathLoop( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  using namespace StoredPiecesJumps::Loop;

  TrackBp::Vertex nodeBeg = mGraph.vertex( aEdge.vb );
  TrackBp::Vertex nodeEnd = mGraph.vertex( aEdge.ve );

  float length          = DxMath::distance( nodeEnd.pos, nodeBeg.pos );  
  float lengthTangents  = length*0.1f;
  

  D3DXVECTOR3 tan = nodeBeg.tan;
  D3DXVECTOR3 nor = nodeBeg.nor;
  D3DXVECTOR3 bin = DxMath::thirdVectorBasis( nodeBeg.tan, nodeBeg.nor );
  
  TrackPath* trackPath = new TrackPath(aTrackPathDebugId);  
  

  TrackBp::Vertex ps( nodeBeg.pos + tan * psLength,
                      tan,
                      nor );

  TrackBp::Vertex pa( nodeBeg.pos + bin * paWidthC + nor * pHeight + tan * paLength,
                      nor,
                      -tan );

  TrackBp::Vertex pt( nodeBeg.pos + bin * ptWidthC + nor * 20 + tan * ptLength,
                      DxMath::alignToTangents(  D3DXVECTOR3(1,0,0), 
                                                tan, 
                                                DxMath::unitVector( 
                                                  D3DXVECTOR3(-2,1,0),
                                                  D3DXVECTOR3(0,0,0)
                                                )
                                              ),
                      -nor );

  TrackBp::Vertex pb( nodeBeg.pos + bin * pbWidthC + nor * pHeight + tan * pbLength,
                      -nor,
                      tan );

  TrackBp::Vertex pe( nodeBeg.pos + bin * pbWidthC + tan * peLength,
                      tan,
                      nor );
    

  trackPath->addEdge( nodeBeg, ps, lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( ps, pa, lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( pa, pt, lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( pt, pb, lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( pb, pe, lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( pe, nodeEnd, lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
    
  return trackPath;
}
    
TrackPath* Track::generatePathLateral( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  return NULL;
}
    
TrackPath* Track::generatePathLateralCross( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  return NULL;
}
    
TrackPath* Track::generatePathTwoWayJump( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  using namespace StoredPiecesJumps::TwoWayJump;

  TrackBp::Vertex nodeBeg = mGraph.vertex( aEdge.vb );
  TrackBp::Vertex nodeEnd = mGraph.vertex( aEdge.ve );

  float length          = DxMath::distance( nodeEnd.pos, nodeBeg.pos );  
  float lengthTangents  = length*0.1f;

  D3DXVECTOR3 tan = nodeBeg.tan;
  D3DXVECTOR3 nor =  nodeBeg.nor ;
  /*
  D3DXVECTOR3 tan = DxMath::unitVector( nodeEnd.pos, nodeBeg.pos );
  D3DXVECTOR3 nor = DxMath::alignToTangents( tan, nodeBeg.tan, nodeBeg.nor );
  */
  D3DXVECTOR3 bin = DxMath::thirdVectorBasis( tan, nor );
  
    
  D3DXVECTOR3 jumpTangent = (nodeBeg.tan + nodeBeg.nor) * 0.5f;
  D3DXVec3Normalize( &jumpTangent, &jumpTangent );
  D3DXVECTOR3 jumpNormal  = DxMath::alignToTangents( jumpTangent,nodeBeg.tan, nodeBeg.nor );

  TrackBp::Vertex jumpPoint(  nodeBeg.pos + tan*jumpLength + nor*jumpHeight,
                              jumpTangent,
                              jumpNormal );

  TrackBp::Vertex landingPoint1(  nodeBeg.pos + tan*b1Length + nor*b1Height,
                                  tan,
                                  nor );       

  TrackBp::Vertex landingPoint2(  nodeBeg.pos + tan*b2Length + nor*b2Height,
                                  tan,
                                  nor );

  TrackBp::Vertex outPoint1(  nodeBeg.pos + tan*peLength+ nor*b1Height,
                                  tan,
                                  nor );    
  TrackBp::Vertex outPoint2(  nodeBeg.pos + tan*peLength + nor*b2Height,
                                  tan,
                                  nor ); 

  TrackPath* trackPath = new TrackPath(aTrackPathDebugId);

  //  Ramp
  trackPath->addEdge( nodeBeg, jumpPoint, lengthTangents, mSliceNoBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( landingPoint1, outPoint1, lengthTangents, mSliceNoBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );
  trackPath->addEdge( landingPoint2, outPoint2, lengthTangents, mSliceNoBorder, mSliceBorder, aTrackWidth, mSliceMultiplier );  

  return trackPath;
}
    
TrackPath* Track::generatePathThreeWayJump( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  return NULL;
}

TrackPath* Track::generatePathTwister( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  using namespace StoredPiecesJumps::Twister;

  TrackBp::Vertex nodeBeg = mGraph.vertex( aEdge.vb );
  TrackBp::Vertex nodeEnd = mGraph.vertex( aEdge.ve );

  float length          = DxMath::distance( nodeEnd.pos, nodeBeg.pos );  
  float lengthTangents  = length*0.1f;
  

  TrackBp::Vertex t1( nodeBeg.pos + nodeBeg.tan*length*0.2f,
                      nodeBeg.tan,
                      DxMath::normalizeVector( D3DXVECTOR3(0,1,1) ) );

  TrackBp::Vertex t2( nodeBeg.pos + nodeBeg.tan*length*0.4f,
                      nodeEnd.tan,
                      nodeBeg.nor);

  TrackBp::Vertex t3( nodeBeg.pos + nodeBeg.tan*length*0.6f,
                      nodeEnd.tan,
                      DxMath::normalizeVector( D3DXVECTOR3(0,1,-1) ));
                                
 TrackBp::Vertex  t4( nodeBeg.pos + nodeBeg.tan*length*0.8f,
                      nodeEnd.tan,
                      nodeEnd.nor);

  TrackPath* trackPath = new TrackPath(aTrackPathDebugId);

  //  Ramp
  trackPath->addEdge( nodeBeg, t1,  lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier);
  trackPath->addEdge( t1, t2,       lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier);
  trackPath->addEdge( t2, t3,       lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier);
  trackPath->addEdge( t3, t4,       lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier);
  trackPath->addEdge( t4, nodeEnd,  lengthTangents, mSliceBorder, mSliceBorder, aTrackWidth, mSliceMultiplier);
  
  return trackPath;
}
