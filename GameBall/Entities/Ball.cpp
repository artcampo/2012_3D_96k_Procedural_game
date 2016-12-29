#include  "GameBall/Track/TrackPathIA.hpp"
#include  "GameBall/Entities/Ball.hpp"
#include  "GameBall/Track/TrackEdge.hpp"
#include  "GameBall/Track.hpp"


#define _BP_EXTRA_LOG

Ball::~Ball()
{
  delete mBallModel;
  delete mLog;
}

Ball::Ball (  Physics* aPhysics,
              const GameBp::NitroDescription& aNitroDesc, 
              const GameBp::BallDescription&  aBallDesc,
              const D3DXVECTOR3& aInitPosition, 
              const int aBall, 
              ID3D10ShaderResourceView* aPermTextureSRV )
{
  mNitroDesc        = aNitroDesc;
  mBallDesc         = aBallDesc;  
  mNitroUsed        = false;
  mNumNitro         = mNitroDesc.NumStartingNitros;

  if ( aBall == GameBp::eBallMetal) mBallModel  = new BallMetal(aPermTextureSRV);
  if ( aBall == GameBp::eBallGlass) mBallModel  = new BallGlass(aPermTextureSRV);
  if ( aBall == GameBp::eBallBouncy) mBallModel  = new BallBouncy(aPermTextureSRV);
  if ( aBall == GameBp::eBallMeteor) mBallModel  = new BallMeteor(aPermTextureSRV);
  if ( aBall == GameBp::eBallDemo) mBallModel  = new BallDemo(aPermTextureSRV);
  if ( aBall == GameBp::eBallChase) mBallModel  = new BallChase(aPermTextureSRV);
  if ( aBall == GameBp::eBallSpartacus) mBallModel  = new BallSpartacus(aPermTextureSRV);

  mLog              = new Log();
  mCurrentMaxVelocity = aBallDesc.maxVelocity;

  Global::mNitros = mNumNitro;
  //TODO: PHYS
  mBallBody = aPhysics->createRigidBody( aInitPosition, 2.5f, 0.25, 0.5, 0.2 );

  mBallBody->setMaxVelocity( aBallDesc.maxVelocity );
 
  mBallTForceForw =
    mBallBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), 
                                        &gShipTForceFwdSpline, mBallDesc.fwdModule );

  mBallTForceBack =
    mBallBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( -1.0f, 0.0f, 0.0f), 
                                        &gShipTForceFwdSpline, mBallDesc.bckModule );  
  
  mBallTForceLeft = 
    mBallBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( 0.0f, 0.0f, 1.0f), 
                                        &gShipTForceFwdSpline, mBallDesc.sidModule );

  mBallTForceRight =
    mBallBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( 0.0f, 0.0f, -1.0f), 
                                        &gShipTForceFwdSpline, mBallDesc.sidModule  );


  //////////////////////////////////////////////////////////
  //    Init Ship Torques
  mBallTTorqueUp  = 
    mBallBody->mBodyExt->attachTimeTorque( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );

  mBallTTorqueDown  = 
    mBallBody->mBodyExt->attachTimeTorque( &D3DXVECTOR3( -1.0f, 0.0f, 0.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );

  mBallTTorqueLeft  =
    mBallBody->mBodyExt->attachTimeTorque( &D3DXVECTOR3( 0.0f, 0.0f, -1.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );    

  mBallTTorqueRight  = 
    mBallBody->mBodyExt->attachTimeTorque( &D3DXVECTOR3( 0.0f, 0.0f, 1.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );  
     
}

void  Ball::inGameReset()
{
  clearForces();
  resetMovementVector();
}

void  Ball::resetMovementVector()
{
  //  Set Camera
  mLastNormalOfContact = D3DXVECTOR3(0,1,0); 
  mLastVelocityNonZero = D3DXVECTOR3(1,0,0); 
  mPointingDirection   = D3DXVECTOR3(1,0,0);
  D3DXVec3Cross( &mLastCrossProd, &mPointingDirection, &mLastNormalOfContact );
}

void  Ball::update( const float aLapsedTime, const Track* aTrack )
{
  //  added for some tricks
  EdgePath* edge      = findNearestEdgePath( 100, aTrack );
  float pathT         = findNearestPointInPath( edge );

  if ( mNitroUsed )
  {
    mLapsedTimeSinceNitro += aLapsedTime;
    if ( mLapsedTimeSinceNitro > mNitroDesc.NitroTimeRecharge )
    {
      mNitroUsed = false;
    }
  }
 
  float currentCoef   = 0.0f;
  std::vector<bool> erase ( mVelModifiers.size() );
  for ( int i = 0; i < mVelModifiers.size() ; ++i )
  {
    mVelModifiers[i].LapsedTime += aLapsedTime;
    if ( mVelModifiers[i].LapsedTime > mVelModifiers[i].MaxLapsedTime )
      erase[i] = true;
    else
    {
      erase[i] = false;
      currentCoef += mVelModifiers[i].coefficient;
      #ifdef _BP_EXTRA_LOG
        *mLog->mFile << "Nitro Wait: "<<mVelModifiers[i].LapsedTime<<"/"<< mVelModifiers[i].MaxLapsedTime<< std::endl;
      #endif
    }

  }
  
  mTargetMaxVelocity = mBallDesc.maxVelocity + mBallDesc.maxVelocity*currentCoef;
  float incVel = (mTargetMaxVelocity - mCurrentMaxVelocity)*aLapsedTime*mBallDesc.maxVelTimeCoef;

  *mLog->mFile_Ball[mBallBody->mId] << "Current: "<<mCurrentMaxVelocity <<" Target: "<<mTargetMaxVelocity;
  *mLog->mFile_Ball[mBallBody->mId] << " Inc: "<<incVel << std::endl;

  mCurrentMaxVelocity = mCurrentMaxVelocity + incVel;
  mBallBody->setMaxVelocity( mCurrentMaxVelocity );

  
  #ifdef _BP_EXTRA_LOG
    *mLog->mFile << "Coef: "<<currentCoef <<" Vmax:"<<mCurrentMaxVelocity << std::endl;
  #endif

  for ( int i = mVelModifiers.size() - 1; i >= 0  ; --i )
  {
    if ( erase[i] )
      mVelModifiers.erase (mVelModifiers.begin() + i);
  }

  updateCameraParameters( aLapsedTime );

  if ( mBallBody->isInRestingContact() )
  {        
    if ( !DxMath::equality( mBallBody->getvWS(), D3DXVECTOR3(0,0,0), 0.01f ) )
    {
      //rotate( mBallBody->getvWS(), mBallBody->mContactNormal[0], aLapsedTime );

      mLastRotationV = mBallBody->getvWS();
      mLastRotationN = mBallBody->mContactNormal[0];
    }   
  }
  else
  {
    //  Ball is in the air (just like love)
    /*
    if ( !DxMath::equality( mLastRotationV, D3DXVECTOR3(0,0,0), 0.01f ) )
      rotate( mLastRotationV, mLastRotationN, aLapsedTime );
      */
  }
  
}

void Ball::rotate( const D3DXVECTOR3 aV, const D3DXVECTOR3 aN, const float aLapsedTime )
{
  return;
  float r = mBallBody->mBodyExt->mBoundingSphereRadius;

  D3DXVECTOR3 axisRotation;
  D3DXVec3Cross( &axisRotation, &aV, &aN );
  D3DXVec3Normalize( &axisRotation, &axisRotation );

  D3DXQUATERNION currentRotation;  
  float anglesRotationTotal = D3DXVec3Length( &aV ) * aLapsedTime / -1000;

  for ( int i = 0; i < mRotationInfinitesimalSteps; ++ i)
  {
    float anglesRotation = anglesRotationTotal / float(mRotationInfinitesimalSteps);
    D3DXQuaternionRotationAxis( &currentRotation, &axisRotation, anglesRotation );
    D3DXQuaternionMultiply( &mBallBody->qWS, &mBallBody->qWS, &currentRotation );
  }
}

void Ball::updateCameraParameters( const float aLapsedTime )
{
  mBallBody->computeTemporalValues();
//  Update application vectors
  bool  shipBaseChanged = false;
  if ( mBallBody->hasUpwardsNormalOfContact() )
  {
    mLastNormalOfContact = mBallBody->upwardsNormalOfContact();
    shipBaseChanged = true;
  }

  D3DXVECTOR3 v = mBallBody->getvWS();
  v.y = 0.0f;   
  if ( !DxMath::equalsZero( v, 1.0f ) ) 
  {
    float module = D3DXVec3Length( &v );
    //D3DXVec3Normalize( &v, &v );
    D3DXVECTOR3 lastVelocityNonZeroUnit;
    D3DXVec3Normalize( &lastVelocityNonZeroUnit, &v );
    //if ( fabs(lastVelocityNonZeroUnit.y) < 0.5f)
    {
      mLastVelocityNonZero  = v;            
      D3DXVECTOR3 delta = lastVelocityNonZeroUnit - mPointingDirection;
      //delta *= module*aLapsedTime*0.1f;
      delta *= module*0.01f;
      *Global::mFileDebugCamera << "t: " << aLapsedTime << std::endl;      
      *Global::mFileDebugCamera << "mod: " << module << std::endl;      

      mPointingDirection    += (delta);
      shipBaseChanged       = true;
    }
  }

  if ( shipBaseChanged )
  {
    D3DXVECTOR3 pointingUnit;
    D3DXVec3Normalize( &pointingUnit, &mPointingDirection );
    
    
    D3DXVec3Cross( &mLastCrossProd, &pointingUnit, &D3DXVECTOR3(0,1,0) );
    D3DXVec3Normalize( &mLastCrossProd, &mLastCrossProd );

    mBallTForceForw->setApplicationVector ( pointingUnit );
    mBallTForceBack->setApplicationVector ( -1.0f*pointingUnit );
    mBallTForceLeft->setApplicationVector ( mLastCrossProd );
    mBallTForceRight->setApplicationVector( -1.0f*mLastCrossProd );
    
/*
    //*Global::mFileDebugCamera << "Id: " << mBallBody->mId << std::endl;      
    *Global::mFileDebugCamera << "V: " << v.x << "," <<v.y<<","<<v.z<<std::endl;
    *Global::mFileDebugCamera << "Pointing T: " << pointingUnit.x << "," <<pointingUnit.y<<","<<pointingUnit.z<<std::endl;
    *Global::mFileDebugCamera << "Pointing N: " << mLastCrossProd.x << "," <<mLastCrossProd.y<<","<<mLastCrossProd.z<<std::endl<<std::endl;
    *Global::mFileDebugCamera << std::endl;      
    */

  }
}

bool Ball::allowsNitro()
{
  return (!mNitroUsed && (mNumNitro > 0 ) );
}

void Ball::applyNitro()
{
  
  --mNumNitro;
  mNitroUsed            = true;
  mLapsedTimeSinceNitro = 0.0f;

  D3DXVECTOR3 v;

  v = mBallBody->getvWS();
  v += v*mNitroDesc.NitroCoefImpulse;
  mBallBody->setVelWS( v );

  GameBp::VelModifier velm;
  velm.coefficient    = mNitroDesc.NitroCoefMaxVel;
  velm.MaxLapsedTime  = mNitroDesc.NitroTimeVelChange;
  velm.LapsedTime     = 0.0f;

  mVelModifiers.push_back( velm );
  //update( 0.0f );   //WHAT WAS THAT?!?!

#ifdef _BP_EXTRA_LOG
  *mLog->mFile << "Nitro applied" << std::endl;
#endif

  Global::mNitros = mNumNitro;
}

void Ball::addNitro()
{
  ++mNumNitro;
  Global::mNitros = mNumNitro;
}

void Ball::maxVelChange( const float aInc, const float aTime  )
{
  GameBp::VelModifier velm;
  velm.coefficient    = aInc;
  velm.MaxLapsedTime  = aTime;
  velm.LapsedTime     = 0.0f;

  mVelModifiers.push_back( velm );
  //update( 0.0f );
}

void  Ball::clearForces()
{
  mBallTForceForw->initialState();
  mBallTForceBack->initialState();
  mBallTForceLeft->initialState();
  mBallTForceRight->initialState();

  mBallTTorqueUp->initialState();
  mBallTTorqueDown->initialState();
  mBallTTorqueLeft->initialState();
  mBallTTorqueRight->initialState();
}

D3DXVECTOR3 Ball::pointingUnit() const
{
  D3DXVECTOR3 pointingUnit; 
  D3DXVec3Normalize( &pointingUnit, &mPointingDirection );
  return pointingUnit;
}

void  Ball::moveFwd( const bool aMove )
{
  TimeForceState state;
  state.t         = 1.0f;
  state.advancing = aMove; 
  mBallTForceForw->updateState( state );
}
void  Ball::moveBck( const bool aMove )
{
  TimeForceState state;
  state.t         = 1.0f;
  state.advancing = aMove; 
  mBallTForceBack->updateState( state );
}
void  Ball::moveLft( const bool aMove )
{
  TimeForceState state;
  state.t         = 1.0f;
  state.advancing = aMove; 
  mBallTForceLeft->updateState( state );
}
void  Ball::moveRgt( const bool aMove )
{
  TimeForceState state;
  state.t         = 1.0f;
  state.advancing = aMove; 
  mBallTForceRight->updateState( state );
}

D3DXVECTOR3 Ball::movementVector() const
{
  D3DXVECTOR3 vel = mBallBody->getvWS();
  if ( DxMath::equalsZero( vel ) )
    return pointingUnit();
  else 
    return vel;
}

void  Ball::display() const
{

    
    D3DXMatrixTranslation(  &Global::mWorld, 
                            mBallBody->xWS.x,
                            mBallBody->xWS.y,
                            mBallBody->xWS.z);

    D3DXMATRIX matrixRotation;

    D3DXMatrixRotationQuaternion( &matrixRotation, &mBallBody->qWS );

    Global::mWorld = matrixRotation*Global::mWorld;
    D3DXMATRIX m;
    //D3DXMatrixScaling(&m, 0.1, 0.1, 0.1);
    D3DXMatrixScaling(&m, 0.2, 0.2, 0.2);
    Global::mWorld = m*Global::mWorld;    
    
    mBallModel->display();
}

int   Ball::id() const
{
  return mBallBody->mId;
}

EdgePath*  Ball::findNearestEdgePath( const float aDistanceThreshold, const Track* aTrack )
{

  Kdd2d::KddTree2d<EdgePath>* edgePathsKdd = aTrack->edgePathsKdd();

  D3DXVECTOR3 ballp = mBallBody->getxWS();

  std::vector<EdgePath*> nearestEdges = 
      edgePathsKdd->neighbours( ballp.x, 
                                ballp.z,
                                aDistanceThreshold );

  //  Increase threshold until at least one found
  if ( nearestEdges.size() == 0 )
    return findNearestEdgePath( aDistanceThreshold*10.0f, aTrack );

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
  
  //mCurrentEdgePath = nearestEdge;
  //Global::gIAedge = nearestEdge->tPath->mId;
  //mCheckForNearestEdge = false;
  //mPathTargetT = 0.0f;
  
  
  return nearestEdge;
}

TrackPath*  Ball::nearestTrackPath( const Track* aTrack )
{
  return findNearestEdgePath( 100, aTrack)->tPath;
}

float Ball::findNearestPointInPath( const EdgePath* aEdge )
{
  TrackEdge*  edge  = aEdge->tEdge;
  D3DXVECTOR3 ballp = mBallBody->getxWS();

  float t = edge->mSplinePath->nearestPathPoint( ballp, 500 );
 // mDebugPathPoint = edge->mSplinePath->getPoint( t );
  return t;
}

TrackEdge*  Ball::nearestEdgePath ( const Track* aTrack )
{
  return findNearestEdgePath( 100, aTrack)->tEdge;
}

float  Ball::distanceToEdgePath ( const TrackEdge* aEdge )
{
  float t = aEdge->mSplinePath->nearestPathPoint( mBallBody->getxWS(), 500 );
  D3DXVECTOR3 nearestPathPoint = aEdge->mSplinePath->getPoint( t );
  return DxMath::distance( mBallBody->getxWS(), nearestPathPoint );
}