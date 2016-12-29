#include  "GameBall/Modes/GameChase.hpp"
#include  "GameBall\Modes\Chase\GameChaseStateEnd.hpp"

GameChase::GameChase( const int aDifficulty ) :
  mTargetLineModel(NULL),
    mIaExtraLineModel(NULL)
  
{  
  mDifficulty           = aDifficulty;
  loadDifficultyFile ( "difct_cfg.txt" );
  mMode = GameOptions::eModeChase;  
}

void GameChase::_initStates()
{
  mGameContext = new GameChaseContext( mCam, this);
}

GameChase::~GameChase()
{  
  delete mChaseBall;
  delete mGameContext;
}

IoAbstract*  GameChase::handlerIOinput() 
{
  return mGameContext->_stateHandlerIOinput();
}

void GameChase::_setCamera()
{
  setCamera3rdPerson();
}

void GameChase::_updateCamera( const float aLapsedTime )
{
  mCam3dPerson->updateTargetParameters( mChaseBall->mBallBody->xWS, 
    mChaseBall->pointingUnit(), 
    mChaseBall->mLastNormalOfContact,
    mChaseBall->mBallBody->getvWS(),
    aLapsedTime );  
  //mCam3dPerson->updateTargetParameters( mBall->mBallBody->xWS, mBall->pointingUnit(), mBall->mBallBody->getvWS(), aLapsedTime );  
  
  mCam3dPerson->updateCamera();

  
}

void GameChase::_update( const float aLapsedTime )
{
  mGameContext->_stateUpdate(aLapsedTime);
}

void GameChase::_initTrackDesc()
{  
  mTrackDesc.totalLength  = mLoaded_trackLength;
  
  mTrackDesc.curvatureMin = 1;
  mTrackDesc.curvatureMin = 2;
  mTrackDesc.numUnitsPerCurve = 50;
  mTrackDesc.iterations = mLoaded_diff_Track_Num_Intersections[mDifficulty];

  mTrackDesc.usesIA = true;

  mTrackDesc.trackWidth = mLoaded_diff_Track_Width[mDifficulty];
  mTrackDesc.trackWidthEntities = mLoaded_diff_Track_Width_Entities[mDifficulty];
}

void GameChase::_initGameState()
{
  //mChaseBall->mBallBody->xWS = D3DXVECTOR3( 359.8009, 0.6139, -30.5144 );
  mChaseBall->mBallBody->standstill();
  mChaseBall->inGameReset();
}

void GameChase::_initEntityDesc()
{
  mTrackEntDesc.archTimeUpUsed = false;
}

void GameChase::_checkEndGame( const float aLapsedTime )
{  
  float d = DxMath::distance( mChaseBall->mBallBody->xWS,
                              mBall->mBallBody->xWS);
  if ( d < 1 ) 
  {
    mGameWon = true;
    mGameContext->currentState( mGameContext->mStateEnd );    
  }

  mEnd->cross( mChaseBall->mBallBody->getxWS(), *mChaseBall );
  mEnd->update( aLapsedTime );
}

void GameChase::_triggerEnd(Ball &aBall)
{
  if ( aBall.id() == mChaseBall->id() )
  {
    mGameWon = false;
    mGameContext->currentState( mGameContext->mStateEnd );    
  }
}


void GameChase::_initBalls()
{
  GameBp::BallDescription ballDesc;
  ballDesc.idBall       = mUser_idBall;
  ballDesc.maxVelocity  = mLoaded_maxVelocity;
  ballDesc.fwdModule    = mLoaded_FwdModule;
  ballDesc.bckModule    = mLoaded_BckModule; 
  ballDesc.sidModule    = mLoaded_SidModule;
  ballDesc.maxVelTimeCoef = 0.3f;


  D3DXVECTOR3 chaseBallPos = mLoadedInitPosition + D3DXVECTOR3(5,0,1);
  mChaseBall = new Ball( mPhysics, mNitroDesc, ballDesc, chaseBallPos, GameBp::eBallMetal, mPermTextureSRV );
  mBalls.push_back( mChaseBall );

  
#ifndef GBP_GFXDEBUG_TRACK
   mChaseBall_IA = new Ball_IA( *mChaseBall, mPhysics, *mTrack );
#endif
#ifdef GBP_GFXDEBUG_TRACK
   mChaseBall_IA = new Ball_IA( *mChaseBall, mPhysics, *mTrack[0] );
#endif 
}

void GameChase::_IAstep( const float aLapsedTime )
{
  mChaseBall_IA->update( aLapsedTime );
}

void GameChase::_drawScene()
{
  if ( mTargetLineModel != NULL )
    delete mTargetLineModel;

  std::vector<VertexPos> vertices = mChaseBall_IA->getRenderTargetPoints();
  D3DXCOLOR col = D3DXCOLOR(1,0,0,1);
  mTargetLineModel = new LineListModel<VertexPos>( vertices, col );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
  mTargetLineModel->display();

  
  {
    if ( mIaExtraLineModel != NULL )
      delete mIaExtraLineModel;

    std::vector<VertexPos> vertices = mChaseBall_IA->getRenderTargetExtraPoints();
    D3DXCOLOR col = D3DXCOLOR(1,0,0,1);
    mIaExtraLineModel = new LineListModel<VertexPos>( vertices, col );
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
    mIaExtraLineModel->display();
  }


  mGameContext->_stateDrawScene();
}


std::string  GameChase::_getMainMessage() const
{
  return mGameContext->mCurrentState->_getMainMessage();
}