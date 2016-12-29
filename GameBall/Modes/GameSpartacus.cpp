#include  "GameBall/Modes/GameSpartacus.hpp"
#include  "Project_Dependant/IOAbstract.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusContext.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusStateEnd.hpp"

GameSpartacus::GameSpartacus( const int aDifficulty ) :
  mTargetLineModel(NULL)
  
{ 
  mDifficulty           = aDifficulty;
  loadDifficultyFile ( "difct_cfg.txt" );  
  mMode                 = GameOptions::eModeChase;
  mNumBalls             = mLoaded_diff_Spartacus_Num_Opponents[mDifficulty];
  mNumBallsStillOnRace  = mNumBalls;  
}

void GameSpartacus::_initStates()
{
  mGameContext = new GameSpartacusContext( mCam, this);
}

GameSpartacus::~GameSpartacus()
{  
  for ( int i = 0; i < mNumBalls ; ++i )
  {
    delete mChaseBall[i];
    delete mChaseBall_IA[i];
  }
  delete mGameContext;
}

IoAbstract*  GameSpartacus::handlerIOinput() 
{
  return mGameContext->_stateHandlerIOinput();  
}

void GameSpartacus::_setCamera()
{
  setCamera3rdPerson();
  
}

void GameSpartacus::_updateCamera( const float aLapsedTime )
{
  //mCam3dPerson->updateTargetParameters( mChaseBall[1]->mBallBody->xWS, mChaseBall[1]->pointingUnit(), mChaseBall[1]->mBallBody->getvWS(),aLapsedTime );  
  mCam3dPerson->updateTargetParameters( mBall->mBallBody->xWS, mBall->pointingUnit(), mBall->mLastNormalOfContact, mBall->mBallBody->getvWS(), aLapsedTime );  
  
  mCam3dPerson->updateCamera();
  //mGameContext->_stateUpdate(aLapsedTime);
}

void GameSpartacus::_initTrackDesc()
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

void GameSpartacus::_initGameState()
{

  for ( int i = 0; i < mNumBalls ; ++i )
  {
    mChaseBall[i]->mBallBody->standstill();
    mChaseBall[i]->inGameReset();
    mSpartacusBallLapsedTimeSinceInContact[i] = 0;
    mSpartacusBallInRace[i] = true;
  }
}

void GameSpartacus::_initEntityDesc()
{
  mTrackEntDesc.archTimeUpUsed = false;
}

void GameSpartacus::_checkEndGame( const float aLapsedTime )
{  
}

void GameSpartacus::_triggerEnd(Ball &aBall)
{
  int id = aBall.id();
  if( id != 0 && mSpartacusBallInRace[id] )
  {
    actionSpartacusBallOutOfRace(id - 1);
    if ( mNumBallsStillOnRace == 0 )
    {
      mGameWon = false;
      mGameContext->currentState( mGameContext->mStateEnd );    
    }
  }  
    
}


void GameSpartacus::_initBalls()
{
  GameBp::BallDescription ballDesc;
  ballDesc.idBall       = mUser_idBall;
  ballDesc.maxVelocity  = mLoaded_maxVelocity;
  ballDesc.fwdModule    = mLoaded_FwdModule;
  ballDesc.bckModule    = mLoaded_BckModule; 
  ballDesc.sidModule    = mLoaded_SidModule;
  ballDesc.maxVelTimeCoef = 0.3f;

  mChaseBall.resize( mNumBalls );
  mChaseBall_IA.resize( mNumBalls );
  mTargetLineModel.resize( mNumBalls );
  mSpartacusBallLapsedTimeSinceInContact.resize( mNumBalls );
  mSpartacusBallInRace.resize( mNumBalls );

  for ( int i = 0; i < mNumBalls ; ++i )
  {
    D3DXVECTOR3 chaseBallPos = mLoadedInitPosition + D3DXVECTOR3(5,0,5) + i*D3DXVECTOR3(2,0,2);
    mChaseBall[i] = new Ball( mPhysics, mNitroDesc, ballDesc, chaseBallPos, GameBp::eBallSpartacus, mPermTextureSRV );
    mBalls.push_back( mChaseBall[i] );

#ifndef GBP_GFXDEBUG_TRACK
      mChaseBall_IA[i] = new Ball_IA( *mChaseBall[i], mPhysics, *mTrack );
#endif
#ifdef GBP_GFXDEBUG_TRACK
      mChaseBall_IA[i] = new Ball_IA( *mChaseBall[i], mPhysics, *mTrack[0] );
#endif 

    mTargetLineModel[i] = NULL;
  }
}

void GameSpartacus::_IAstep( const float aLapsedTime )
{
  for ( int i = 0; i < mNumBalls ; ++i )
  {
    if ( mSpartacusBallInRace[i] )
      mChaseBall_IA[i]->update( aLapsedTime );
  }
}

void GameSpartacus::_drawScene()
{
  for ( int i = 0; i < mNumBalls ; ++i )
  {
    if ( mTargetLineModel[i] != NULL )
      delete mTargetLineModel[i];

    std::vector<VertexPos> vertices(2);
    vertices[0].pos = mChaseBall[i]->mBallBody->getxWS();
    vertices[1].pos = mChaseBall_IA[i]->mTargetPoint;
    D3DXCOLOR col = D3DXCOLOR(1,0,0,1);
    mTargetLineModel[i] = new LineListModel<VertexPos>( vertices, col );
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
    mTargetLineModel[i]->display();
  }

  mGameContext->_stateDrawScene();
}


void GameSpartacus::_update( const float aLapsedTime )
{
  mGameContext->_stateUpdate(aLapsedTime);

  for ( int i = 0; i < mNumBalls ; ++i )
  {
    if ( mSpartacusBallInRace[i] )
    {
      if ( mChaseBall[i]->mBallBody->isInRestingContact() )
        mSpartacusBallLapsedTimeSinceInContact[i] = 0.0f;
      else
      {
        mSpartacusBallLapsedTimeSinceInContact[i] += float(aLapsedTime);
        
        if ( mSpartacusBallLapsedTimeSinceInContact[i] > (12.0f*100.0f) )
        {
          *mLog->mFileEvents << "Spartacus ball out, index: "<< i << " [" << mPhysics->mDebugFrame << "]" << std::endl;
          actionSpartacusBallOutOfRace(i);
          if ( mNumBallsStillOnRace == 0 )
          {
            mGameWon = true;
            mGameContext->currentState( mGameContext->mStateEnd );    
          }
        }
      }  
    }
  }
}

void      GameSpartacus::actionSpartacusBallOutOfRace( const int aIndex )
{
  mSpartacusBallInRace[aIndex] = false;
  --mNumBallsStillOnRace;

}

void GameSpartacus::_drawBalls()
{
  mBall->display();
  for ( int i = 0; i < mNumBalls ; ++i )
  {
    if ( mSpartacusBallInRace[i] )
      mChaseBall[i]->display(); 
  }
}

int       GameSpartacus::numBallsInRace() const
{
  return mNumBallsStillOnRace;
}

std::string  GameSpartacus::_getMainMessage() const
{
  return mGameContext->mCurrentState->_getMainMessage();
}

