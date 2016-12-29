#include  "GameBall/Track.hpp"


//  Returns NULL if doesn't generate geometry
TrackPath* Track::generatePath( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  using namespace TrackBp;

  if ( aEdge.edgeData.isModifiable() )
  {
    mLog->mProfGeom->start( std::string("Integrate") );
    TrackPath* path = generatePathSansCurves( aTrackPathDebugId, aTrackWidth, aEdge );
    float l = path->integrateLength();
    float differenceEpsilon = aEdge.edgeData.mDesiredLength*mEpsilonDesiredLengthEquality;
    delete path;
    mLog->mProfGeom->end();

    if ( DxMath::equality( l, aEdge.edgeData.mDesiredLength, differenceEpsilon ) )  
      return generatePathSansCurves( aTrackPathDebugId, aTrackWidth, aEdge );  
    else  
      return generatePathCurvy( aTrackPathDebugId, aTrackWidth, aEdge );
  }
  else
  {
    int a;
    switch ( aEdge.edgeData.jumpId() )
    {
    case eEdgeRamp: 
      return generatePathRamp( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeNineTurn:
      return generatePathNineTurn( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeOne80Turn:
      return generatePathOne80Turn( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeLoop:
      return generatePathLoop( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeLateral:
      return generatePathLateral( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeLateralCross:
      return generatePathLateralCross( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeTwoWayJump:
      return generatePathTwoWayJump( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeTwister:
      return generatePathTwister( aTrackPathDebugId, aTrackWidth, aEdge );

    case eEdgeThreeWayJump:
      return generatePathThreeWayJump( aTrackPathDebugId, aTrackWidth, aEdge );

    default:
    case eEdgeNoGeometry:
      TrackPath* trackPath;
      trackPath = new TrackPath( aTrackPathDebugId );
      return trackPath;
    
    };
  }
  
}



TrackPath* Track::generatePathSansCurves( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  mLog->mProfGeom->start( std::string("Sans curves") );
  std::vector<D3DXVECTOR3> path;

  int numCurves = 0;

  TrackBp::Vertex nodeBeg = mGraph.vertex( aEdge.vb );
  TrackBp::Vertex nodeEnd = mGraph.vertex( aEdge.ve );

  //  Create spline
  int bezierPoints = 4 + 3*numCurves;
  path.resize( bezierPoints );

  //  Compute length
  D3DXVECTOR3 distance = ( nodeEnd.pos - nodeBeg.pos );
  float length          = D3DXVec3Length( &distance ); 
  float lengthTangents  = length*0.4f;
  

  //  Put start and end
  path[0] = nodeBeg.pos;
  path[1] = nodeBeg.pos + lengthTangents*nodeBeg.tan;

  path[bezierPoints - 1] = nodeEnd.pos;
  path[bezierPoints - 2] = nodeEnd.pos - lengthTangents*nodeEnd.tan;

  //  now object
  TrackPath* trackPath = new TrackPath( aTrackPathDebugId );

  std::vector<D3DXVECTOR3>* begSlice;
  std::vector<D3DXVECTOR3>* endSlice;

  if ( mGraph.hasArityTwo( aEdge.vb ) ) 
    begSlice = &mSliceBorder;
  else
    begSlice = &mSliceNoBorder;

  if ( mGraph.hasArityTwo( aEdge.ve ) ) 
    endSlice = &mSliceBorder;
  else
    endSlice = &mSliceNoBorder;

  trackPath->addEdge( path, *begSlice, *endSlice,
                      nodeBeg.tan,
                      nodeEnd.tan,
                      nodeBeg.nor,
                      nodeEnd.nor,
                      mDesc.trackWidth,
                      mSliceMultiplier);

  mLog->mProfGeom->end();
  return trackPath;
}


TrackPath* Track::generatePathCurvy( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge )
{
  mLog->mProfGeom->start( std::string("Curvy set") );
  std::vector<D3DXVECTOR3> path;

  TrackBp::Vertex nodeBeg = mGraph.vertex( aEdge.vb );
  TrackBp::Vertex nodeEnd = mGraph.vertex( aEdge.ve );

  //  Compute length
  D3DXVECTOR3 distance = ( nodeEnd.pos - nodeBeg.pos );
  float length          = D3DXVec3Length( &distance ); 
  float lengthTangents  = length*0.4f;
  

  int numCurves = length / mDesc.numUnitsPerCurve;
  if ( numCurves < 0 ) numCurves = 0;

  //DEBUG
  numCurves = 0;
  //-DEBUG

  //  Create spline
  int bezierPoints = 4 + 3*numCurves;
  path.resize( bezierPoints );

  //  Put start and end
  path[0] = nodeBeg.pos;
  path[1] = nodeBeg.pos + lengthTangents*nodeBeg.tan;

  path[bezierPoints - 1] = nodeEnd.pos;
  path[bezierPoints - 2] = nodeEnd.pos - lengthTangents*nodeEnd.tan;

  

  //  Create info for points of curves
  
  std::vector<D3DXVECTOR3>  pointsTangent ( numCurves );
  std::vector<float>        pointsCompT   ( numCurves );
  for ( int i = 0; i < numCurves; ++i )
  {    
    //D3DXVECTOR3 tan = ( nodeEnd.pos - nodeBeg.pos);
    /*
    D3DXVECTOR3 tan = ( nodeEnd.pos - nodeBeg.pos);
    tan *= (float(i + 1.0f)/float(numCurves + 1));
    D3DXVec3Normalize( &tan, &tan );
    pointsTangent[i] = tan;*/
    pointsTangent[i] = nodeBeg.tan;

    pointsCompT[i] = (float(i + 1.0f)/float(numCurves + 1)) *0.8f + 0.1f;
  }

  D3DXVECTOR3 tangent = DxMath::unitVector( nodeEnd.pos, nodeBeg.pos );
  D3DXVECTOR3 binormal; D3DXVec3Cross( &binormal, &tangent, &nodeBeg.nor );


  //  Now modify curvature
  using namespace Rand;
  RandGen r ( aEdge.edgeData.mSeedForCurves );
     
  //float compT = 0.5f;
  float compN = 0;
  float compB = -0.1f;
  float tanL  = lengthTangents;
  float thi1  = 0;
  float thi2  = 0;

  std::vector< std::vector<float> > parameter ( numCurves );
  std::vector< std::vector<float> > weight    ( numCurves );
  std::vector< std::vector<float> > sign      ( numCurves );

  for ( int i = 0; i < numCurves; ++i )
  {
    parameter[i].push_back( compN );
    parameter[i].push_back( compB );
    parameter[i].push_back( tanL );
    parameter[i].push_back( thi1 );
    parameter[i].push_back( thi2 );

    weight[i].push_back( 0 );
    weight[i].push_back( 0.001 );
    weight[i].push_back( 0 );
    weight[i].push_back( 0.0001 );//weight[i].push_back( 0.001f );
    weight[i].push_back( 0 );

    sign[i].push_back( +1 );
    sign[i].push_back( -1 );
    sign[i].push_back( +1 );
    sign[i].push_back( -1 );
    sign[i].push_back( +1 );
  }



  float differenceEpsilon = aEdge.edgeData.mDesiredLength*mEpsilonDesiredLengthEquality;  
  bool pathFitsDesiredLength = false;

  *mLog->mFileTrack << std::endl;
  *mLog->mFileTrack << "-----------------------------------------------------------------------------------" << std::endl;
  *mLog->mFileTrack << "          Fitting curves" << std::endl;
  *mLog->mFileTrack << "-----------------------------------------------------------------------------------" << std::endl;



  //  Init curves
  for ( int i = 0; i < numCurves; ++i )
  {
    int bezierIndex = 3*i + 3;

    generatePathPoint(  nodeBeg.pos, pointsTangent[i], length, 
                        tangent, nodeBeg.nor, binormal,
                        pointsCompT[i], compN, compB,
                        tanL, thi1, thi2,
                        &path[bezierIndex] ); 
  }

  int numIterationsIntegral = 10; //50
  float prevLength = integralOfPath( path, numIterationsIntegral );      
  *mLog->mFileTrack << "Original: " << prevLength << std::endl;
  *mLog->mFileTrack << "Desired: " << aEdge.edgeData.mDesiredLength << std::endl;
  *mLog->mFileTrack << "Number of curves in path: " << numCurves<< std::endl;
  
  

  mLog->mProfGeom->end();
  mLog->mProfGeom->start( std::string("Curvy fit") );

  
  int numMaxIterations  = 1000;
  //DEBUG  
  numMaxIterations  = 0;
  //-DEBUG
  int numIterations = 0;
  while ( !pathFitsDesiredLength && numIterations < numMaxIterations )
  {
    ++numIterations;
    for ( int i = 0; i < numCurves && !pathFitsDesiredLength ; ++i )
    {
      for ( int j = 0; j < 5 && !pathFitsDesiredLength ; ++j )
      {
        if ( weight[i][j] != 0.0f )
        {
          int bezierIndex = 3*i + 3;

          float factor = (1.0f - float(numIterations)/(1000.0f))*10.0f;
          float inc = weight[i][j]*sign[i][j]*factor*r.randFloat_0_1();
          parameter[i][j] += inc;
          //*mLog->mFileTrack << "[[ "<<i<<", par: "<<j<<"+="<<inc<<" ]] ";

          generatePathPoint(  nodeBeg.pos, pointsTangent[i], length, 
                              tangent, nodeBeg.nor, binormal,
                              pointsCompT[i], parameter[i][0], parameter[i][1],
                              parameter[i][2], parameter[i][3], parameter[i][4],
                              &path[bezierIndex] );  

          float l = integralOfPath( path, numIterationsIntegral );      
          //*mLog->mFileTrack << "length: " << l <<std::endl;

          if ( ((l > aEdge.edgeData.mDesiredLength) && (l > prevLength)) || 
               ((l < aEdge.edgeData.mDesiredLength) && (l < prevLength)) )
          {
            if ( sign[i][j] == -1 ) sign[i][j] = +1;
            else                    sign[i][j] = -1;
            //*mLog->mFile << "("<<j<<" flipped) ";
          }

          prevLength = l;

          //*mLog->mFile <<"["<< j<<"]: "<<l << std::endl;
        }
      }// for each param

      pathFitsDesiredLength = ( DxMath::equality( prevLength, aEdge.edgeData.mDesiredLength, differenceEpsilon ) );

    }// for each node

    //pathFitsDesiredLength = ( DxMath::equality( prevLength, aEdge.edgeData.mDesiredLength, differenceEpsilon ) );

  }

  *mLog->mFileTrack << "Num Iterations:  " <<numIterations<< std::endl;
  *mLog->mFileTrack << "With difference:  " <<(aEdge.edgeData.mDesiredLength - prevLength)<< std::endl;
  
  mLog->mProfGeom->end();
  mLog->mProfGeom->start( std::string("Curvy new of object") );

  

  //  now object
  TrackPath* trackPath = new TrackPath(aTrackPathDebugId);

  std::vector<D3DXVECTOR3>* begSlice;
  std::vector<D3DXVECTOR3>* endSlice;

  if ( mGraph.hasArityTwo( aEdge.vb ) ) 
    begSlice = &mSliceBorder;
  else
    begSlice = &mSliceNoBorder;

  if ( mGraph.hasArityTwo( aEdge.ve ) ) 
    endSlice = &mSliceBorder;
  else
    endSlice = &mSliceNoBorder;

  trackPath->addEdge( path, *begSlice, *endSlice,
                      nodeBeg.tan,
                      nodeEnd.tan,
                      nodeBeg.nor,
                      nodeEnd.nor,
                      mDesc.trackWidth,
                      mSliceMultiplier);
  trackPath->debugAddVbId( aEdge.vb );

  mLog->mProfGeom->end();

  return trackPath;
}

void Track::generatePathPoint       ( const D3DXVECTOR3& aPos,
                                      const D3DXVECTOR3& aPointTangent,
                                      const float aBaseLength,
                                      const D3DXVECTOR3& aTan,
                                      const D3DXVECTOR3& aNor,
                                      const D3DXVECTOR3& aBin,
                                      const float aCompT,
                                      const float aCompN,
                                      const float aCompB,
                                      const float aTLength,
                                      const float aThita1,
                                      const float aThita2,
                                      D3DXVECTOR3* aPointAndTangentData )
{
  D3DXVECTOR3 pos = aPos;

  pos += aTan*aCompT*aBaseLength;
  pos += aNor*aCompN*aBaseLength;
  pos += aBin*aCompB*aBaseLength;

  //  Compute tangent
  D3DXVECTOR3 tan;
  D3DXMATRIX  m;
  D3DXMatrixRotationAxis( &m, &aNor, aThita1);
  D3DXVec3TransformCoord( &tan, &aPointTangent, &m );
  D3DXMatrixRotationAxis( &m, &aBin, aThita2);
  D3DXVec3TransformCoord( &tan, &tan, &m );
  D3DXVec3Normalize ( &tan, &tan );
  tan *= aTLength;

  *(aPointAndTangentData - 1) = pos - tan;
  *(aPointAndTangentData + 0) = pos;
  *(aPointAndTangentData + 1) = pos + tan;
}

float Track::integralOfPath ( std::vector<D3DXVECTOR3>& aPath, const int aNumSteps ) 
{
  SplineBezierTMapped *splinePath = new SplineBezierTMapped( &aPath[0], aPath.size(), 15 );
  float length = Spline::integrateLength( splinePath, aNumSteps );
  delete splinePath;
  return length;
}
