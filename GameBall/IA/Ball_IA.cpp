#include  "GameBall/IA/Ball_IA.hpp"
#include  "GameBall/Track.hpp"
#include  "GameBall/Track/TrackPathIA.hpp"
#include  "GameBall/Entities/Ball.hpp"

Ball_IA::~Ball_IA()
{
  delete mLog;
  //delete mRand;
}

Ball_IA::Ball_IA (  Ball& aBall, 
                    Physics* aPhysics, 
                    Track& aTrack ) :
    mBall( aBall ),
    mTrack( aTrack ),
    mCheckForNearestEdge( true ),
    mRand(42),
    mLog(),
    mUnitsAhead(80),    
    mNumLookAheadPoints(4),
    mPathNearestTBias(0.0002f),
    mPathPointSurpassedEpsilon(0.01),
    mPathTargetT(0.0f),
    mPathTargetTEpsilon(0.0f)
{  
}


void  Ball_IA::update( const float aLapsedTime )
{
  EdgePath* edge      = findNearestEdgePath( 100 );
  float pathT         = findNearestPointInPath( edge );
  pathT += mPathNearestTBias;

  mAccelerationMode   = 0;
  

  float bitComp = -1;

  if ( !DxMath::greaterThan( pathT, mPathPreviousT, mPathPointSurpassedEpsilon ) || pathT >= mPathTargetT )
  {
    
    std::vector<BallIA::Target> targets ( mNumLookAheadPoints );
    targets[0] = findNearestTarget( edge, pathT, mUnitsAhead );
    
    float trackWidth;
    {
      TrackEdge*  edgeTrack  = edge->tEdge;
      mDebugPathObstacle = edgeTrack->mSplinePath->getPoint( targets[0].splineT );
      trackWidth = edgeTrack->trackWidth();
    }
    

    for ( int i = 1; i < mNumLookAheadPoints ; ++i )
    {
      targets[i] = findNearestTarget( edge,
                                      targets[i - 1].splineT, 
                                      mUnitsAhead );
    }

    std::vector< std::vector<float> > paths;
    std::vector<float> blankPositionVector;
    createTestPoints( 0, targets, paths, blankPositionVector, trackWidth );

    std::vector<float> pathLength( paths.size() );
    for ( int i = 0; i < paths.size() ; ++i ) 
      //pathLength[i] = computePathChangeAngle( targets, paths[i] );
      //pathLength[i] = computePathLength( targets, paths[i] );
      
      pathLength[i] = computePathChangeAngle( targets, paths[i] ) + 
                      computePathLength( targets, paths[i] );

    
    
    float mind = pathLength[0];
    int mini = 0;

    for ( int i = 1; i < paths.size() ; ++i )
    {
      if ( pathLength[i] < mind )
      {
        mind = pathLength[i];
        mini = i;
      }
    }

    mIaTarget.resize( mNumLookAheadPoints );
    for ( int i = 0; i < mNumLookAheadPoints ; ++i )
      mIaTarget[i] = targets[i].point + targets[i].splineBitangent * paths[mini][i];

    bitComp = paths[mini][0];
    mTargetPoint = 
       (targets[0].point + targets[0].splineBitangent * paths[mini][0]) * 0.9
      +
       (targets[1].point + targets[1].splineBitangent * paths[mini][1]) * 0.075
      +
       (targets[2].point + targets[2].splineBitangent * paths[mini][2]) * 0.025;

    *mLog->mFileIA_Points << "T: " << pathT 
      //<< " Target: " <<mTargetPoint.x<< ", "<<mTargetPoint.y<< ", "<<mTargetPoint.z 
      << " t0: " << targets[0].splineT
      << " t1: " << targets[1].splineT
      << " t2: " << targets[2].splineT
      << " BitCmp: " << bitComp
      << std::endl;

    mPathTargetT = targets[0].splineT + mPathTargetTEpsilon;
    mTargetTangent = mIaTarget[1] - mIaTarget[0];
  }
  else
  {
    *mLog->mFileIA_Points << "-- T: " << pathT << " target: " << mPathTargetT << std::endl;
      //<< " Target: " <<mTargetPoint.x<< ", "<<mTargetPoint.y<< ", "<<mTargetPoint.z 
  }

  mPathPreviousT = pathT;
  

  *mLog->mFileIA  << std::endl;
  *mLog->mFileIA << "t: " << pathT << std::endl;
  //*mLog->mFileIA << "Target: "<<mTargetPoint.x<< ", "<<mTargetPoint.y<< ", "<<mTargetPoint.z << std::endl;
  //*mLog->mFileIA << "Pos: "<<mBall.mBallBody->getxWS().x<< ", "<<mBall.mBallBody->getxWS().y<< ", "<<mBall.mBallBody->getxWS().z << std::endl;
  
  moveToTarget( mTargetPoint );
}

EdgePath*  Ball_IA::findNearestEdgePath( const float aDistanceThreshold )
{
  if ( !mCheckForNearestEdge )
    return mCurrentEdgePath;

  Kdd2d::KddTree2d<EdgePath>* edgePathsKdd = mTrack.edgePathsKdd();

  D3DXVECTOR3 ballp = mBall.mBallBody->getxWS();

  std::vector<EdgePath*> nearestEdges = 
      edgePathsKdd->neighbours( ballp.x, 
                                ballp.z,
                                aDistanceThreshold );

  //  Increase threshold until at least one found
  if ( nearestEdges.size() == 0 )
    return findNearestEdgePath( aDistanceThreshold*10.0f );

  //  find minima
  EdgePath* nearestEdge = nearestEdges[0];
  float mind = nearestEdges[0]->minDistance( ballp );
  for ( int i = 1; i < nearestEdges.size(); ++i )
  {
    if ( nearestEdges[i]->tPath->mId == 52 )
      int a = 2;
    float d = nearestEdges[i]->minDistance( ballp );
    if ( d < mind )
    {
      mind = d;
      nearestEdge = nearestEdges[i];
    }
  }
  
  mCurrentEdgePath = nearestEdge;
  Global::gIAedge = nearestEdge->tPath->mId;
  mCheckForNearestEdge = false;
  mPathTargetT = 0.0f;

  return nearestEdge;
}

float Ball_IA::findNearestPointInPath( const EdgePath* aEdge )
{
  TrackEdge*  edge  = aEdge->tEdge;
  D3DXVECTOR3 ballp = mBall.mBallBody->getxWS();

  float t = edge->mSplinePath->nearestPathPoint( ballp, 500 );
  mDebugPathPoint = edge->mSplinePath->getPoint( t );
  return t;
}

BallIA::Target Ball_IA::findNearestTarget(  const EdgePath* aEdge, 
                                         const float aPathT, 
                                         const float aUnitsAhead)
{
 
  TrackEdge*  edge  = aEdge->tEdge;


  float maxAngle = DxMath::degToRad( 10 );  
  float targetT;
  float advancedUnits;
  bool maxAngleSurpassed;
  BallIA::Target target;
  bool inside = edge->advanceAlongPathNunits( aUnitsAhead, 
                                              aPathT, 
                                              targetT, 
                                              advancedUnits,
                                              maxAngleSurpassed,
                                              maxAngle,                                                            
                                              mBall.mBallBody->getxWS(),
                                              mBall.movementVector(),
                                              target);


  
  if (! inside )
  {    
    float parentT = targetT;
    //  If we have reached end simply go for last point
    if ( aEdge->tPath->numDescendants() == 0 )
    {
      target.hasEntity = false;
      target.point = edge->mSplinePath->getPoint( 1 ); 
      return target;
    }

    TrackPath* children   = aEdge->tPath->descendant( 0 );
    TrackEdge* edgeChild  = children->mEdges[0];
    bool maxAngleSurpassed;
    
    edgeChild->advanceAlongPathNunits( aUnitsAhead - advancedUnits, 
                                        0, 
                                        targetT, 
                                        advancedUnits,
                                        maxAngleSurpassed,
                                        maxAngle,                                                    
                                        mBall.mBallBody->getxWS(),
                                        mBall.movementVector(),
                                        target);

    if ( parentT > 0.98f );
    {
      mCurrentEdgePath->tPath = children;
      mCurrentEdgePath->tEdge = edgeChild;
      Global::gIAedge = mCurrentEdgePath->tPath->mId;
      *mLog->mFileIA_Branch  << "New path "<<Global::gIAedge<< std::endl;
    }
    
  }

  return target;
}

/*
  Changes forces applied on body as to get a little bit closer
  to target.
*/
void  Ball_IA::moveToTarget( const D3DXVECTOR3& aTargetPoint )
{
  D3DXVECTOR3 ballp = mBall.mBallBody->getxWS();
  D3DXVECTOR3 ballv = mBall.mBallBody->getvWS();
  if ( DxMath::equalsZero( ballv ) )
    ballv = mBall.pointingUnit();

  float ballVmodule = D3DXVec3Length( &ballv );

  D3DXVECTOR3 targetVelocity = aTargetPoint - ballp;
  D3DXVECTOR3 projection = DxMath::projectionAgainstVector( ballv, targetVelocity );
  D3DXVECTOR3 perpendicularV = targetVelocity - projection;
  DxMath::clean( perpendicularV, 0.001f );

  D3DXVECTOR3 projBallv = ballv;  projBallv.y = 0;
  D3DXVECTOR3 projTargetVelocity = targetVelocity;  projTargetVelocity.y = 0;

  float deg         = DxMath::radToDeg( DxMath::angleBetweenVectors( projBallv, projTargetVelocity ) );
  float degTangent  = DxMath::radToDeg( DxMath::angleBetweenVectors( projBallv, mTargetTangent ) );
  bool  wholeTurn   = ( deg > 90 );

  //  Choose acceleration mode given targetPoint
  float distance = DxMath::distance( ballp, aTargetPoint );
  
  mAccelerationMode = 3;
  /*
  if ( (deg > 5  || degTangent > 15) && ballVmodule > 20 )   mAccelerationMode = 2;
  if ( (deg > 10 || degTangent > 25) && ballVmodule > 20 )   mAccelerationMode = 1;
  if ( (deg > 20 || degTangent > 45) && ballVmodule > 20 )   mAccelerationMode = 0;
  */

  degTangent = 0;
  if ( (deg > 5  || degTangent > 15) && ballVmodule > 20 )   mAccelerationMode = 2;
  if ( (deg > 10 || degTangent > 25) && ballVmodule > 20 )   mAccelerationMode = 1;
  if ( (deg > 20 || degTangent > 45) && ballVmodule > 20 )   mAccelerationMode = 0;

  if ( wholeTurn )  mAccelerationMode = 1;

  //  Choose to which side move if any
  bool  straight;
  bool  leftOrRight;  
  
  if ( deg < 2 )
    straight = true;
  else
  {
    straight = false;    
    float det = DxMath::orientationAgainst2dVector( D3DXVECTOR2( ballv.x, ballv.z),
                                                    D3DXVECTOR2( projection.x, projection.z),
                                                    D3DXVECTOR2( perpendicularV.x, perpendicularV.z) );

    (det > 0.0f ) ? leftOrRight = true : leftOrRight = false;    
  }
/*
  if ( ballVmodule < 50 )
  {
      mBall.moveFwd( true );
      mBall.moveBck( false );
      mAccelerationMode = 1;
  }
  else*/
  {
    if ( mAccelerationMode == 2 ||  mAccelerationMode == 3 )
    {
      if ( mAccelerationMode >= 3 && mBall.allowsNitro() )
        mBall.applyNitro();

      mBall.moveFwd( true );
      mBall.moveBck( false );
    }
    else
    {
      mBall.moveFwd( false );
      if ( mAccelerationMode == 1 )
        mBall.moveBck( false );
      if ( mAccelerationMode == 0 )
        mBall.moveBck( true );
      
    }
  }

  //  First check counter-direction
  if ( wholeTurn )
  {
    float det = DxMath::orientationAgainst2dVector( D3DXVECTOR2( 0, 0),
                                                    D3DXVECTOR2( ballv.x, ballv.z),
                                                    D3DXVECTOR2( targetVelocity.x, targetVelocity.z) );

    (det > 0.0f ) ? leftOrRight = true : leftOrRight = false;    

    if ( leftOrRight )
    {
      mBall.moveLft( true );
      mBall.moveRgt( false );  
    }
    else
    {
      mBall.moveLft( false );
      mBall.moveRgt( true );  
    }
  }
  else
  {
    //  Good direction, choose orientation
    if ( straight )
    {
      mBall.moveLft( false );
      mBall.moveRgt( false );  
    }
    else
    {
      if ( leftOrRight )
      {
        mBall.moveLft( true );
        mBall.moveRgt( false );  
      }
      else
      {
        mBall.moveLft( false );
        mBall.moveRgt( true );  
      }
    }
  }

  if (mAccelerationMode >= 3 ) Global::gIAvel = std::string("------");
  if (mAccelerationMode == 2 ) Global::gIAvel = std::string("******");
  if (mAccelerationMode == 1 ) Global::gIAvel = std::string("***");
  if (mAccelerationMode == 0 ) Global::gIAvel = std::string("*");
  if (straight )                  Global::gIAdir = std::string("--||--");
  if (!straight && leftOrRight  ) Global::gIAdir = std::string("||----");
  if (!straight && !leftOrRight ) Global::gIAdir = std::string("----||");


  //  Choose how much do we have to accelerate
  
  *mLog->mFileIA  << "Angle: "<< deg<<std::endl;
  *mLog->mFileIA  << "Angle: "<< Global::gAImaxAngle<<std::endl;  


  if ( distance > 0 )  Global::gAIdistance = "dis: *";
  if ( distance > 10 ) Global::gAIdistance = "dis: **";
  if ( distance > 20 ) Global::gAIdistance = "dis: ***";
  if ( distance > 30 ) Global::gAIdistance = "dis: ****";
  if ( distance > 40 ) Global::gAIdistance = "dis: *****";
  if ( distance > 50 ) Global::gAIdistance = "dis: ******";
  if ( distance > 60 ) Global::gAIdistance = "dis: *******";
  if ( distance > 70 ) Global::gAIdistance = "dis: ********";
  if ( distance > 80 ) Global::gAIdistance = "dis: *********";
  
  *mLog->mFileIA  << "Acc: "<< mAccelerationMode<<std::endl;
    
  if ( deg > 0 )   Global::gAImaxAngle = "ang: *";
  if ( deg > 5 )   Global::gAImaxAngle = "ang: **";
  if ( deg > 10 )  Global::gAImaxAngle = "ang: ***";
  if ( deg > 15 )  Global::gAImaxAngle = "ang: ****";
  if ( deg > 30 )  Global::gAImaxAngle = "ang: *****";
  if ( deg > 60 )  Global::gAImaxAngle = "ang: ******";
  if ( deg > 90 )  Global::gAImaxAngle = "ang: RRRRRRR";
  

/*
  *mLog->mFileIA  << "Straight: "<< straight<<std::endl;
  *mLog->mFileIA  << "Left: "<< leftOrRight<<std::endl;
  */  
}


void Ball_IA::createTestPoints( const int aIndex,
                        const std::vector<BallIA::Target>& aTargets, 
                        std::vector< std::vector<float> >& aCombinations,
                        std::vector<float>& aPositions,
                        const float aTrackWidth)
{
  if ( aIndex == aTargets.size() )
  {
    aCombinations.push_back( aPositions );
    return;
  }

  //  construct all possibilities
  int numTestPositions = 6;

  for ( int k = 0; k < numTestPositions ; ++k )
  {
    float bitangentComponent = aTrackWidth * ( float(k) / float(numTestPositions) )
                               - (aTrackWidth*0.5f) ;

    //  Check if position is valid
    bool validBitangentComponent;
    if ( !aTargets[aIndex].hasEntity )
      validBitangentComponent = true;
    else
      validBitangentComponent = !DxMath::equality( bitangentComponent, 
                                                  aTargets[aIndex].entityBitangentComponent, 
                                                  1.5 );

    if ( validBitangentComponent )
    {
      std::vector<float> positions = aPositions;
      positions.push_back( bitangentComponent );
      createTestPoints( aIndex + 1, aTargets, aCombinations, positions, aTrackWidth );
    }
  }
  
}

float Ball_IA::computePathLength( const std::vector<BallIA::Target>& aTargets, 
                        const std::vector<float>& aPositions )
{
  D3DXVECTOR3 p = aTargets[0].point + aPositions[0]*aTargets[0].splineBitangent;
  float distance = DxMath::distance( mBall.mBallBody->getxWS(), p );
  for ( int i = 1; i < aTargets.size() ; ++i )
  {
    D3DXVECTOR3 pPrev = aTargets[i - 1].point + aPositions[i - 1]*aTargets[i - 1].splineBitangent;
    D3DXVECTOR3 pCurr = aTargets[i].point + aPositions[i]*aTargets[i].splineBitangent;

    distance += DxMath::distance( pPrev, pCurr );
  }

  return distance;
}


float Ball_IA::computePathChangeAngle( const std::vector<BallIA::Target>& aTargets, 
                        const std::vector<float>& aPositions )
{  
  float angleSum =  DxMath::angleBetweenVectors( mBall.mBallBody->getvWS(), 
                        aTargets[1].point - aTargets[0].point );
  for ( int i = 1; i < aTargets.size() - 2 ; ++i )
  {
    angleSum += DxMath::angleBetweenVectors( aTargets[i + 1].point - aTargets[i].point, 
                        aTargets[i].point - aTargets[i - 1].point );
  }

  return DxMath::radToDeg( angleSum );
}



std::vector<VertexPos> Ball_IA::getRenderTargetPoints()
{
  std::vector<VertexPos> vertices;

  VertexPos v;
  v.pos = mBall.mBallBody->getxWS();
  vertices.push_back( v );
  v.pos = mIaTarget[0];
  vertices.push_back( v );
  for ( int i = 0; i < mNumLookAheadPoints - 1 ; ++i )
  {
    v.pos = mIaTarget[i];
    vertices.push_back( v );
    v.pos = mIaTarget[i + 1];
    vertices.push_back( v );
  }

  return vertices;
}





std::vector<VertexPos> Ball_IA::getRenderTargetExtraPoints()
{
  std::vector<VertexPos> vertices;

  VertexPos v;

  //  line to spline nearest point
  v.pos = mBall.mBallBody->getxWS();
  vertices.push_back( v );

  v.pos = mDebugPathPoint;
  vertices.push_back( v );  

  //  line to nearest obstacle
  v.pos = mBall.mBallBody->getxWS();
  vertices.push_back( v );

  v.pos = mDebugPathObstacle;
  vertices.push_back( v );  
  

  return vertices;
}