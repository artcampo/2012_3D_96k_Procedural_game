#include  "GameBall/Modes/GameChrono.hpp"
#include  "Project_Dependant/IOAbstract.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall\Modes\Chrono\GameChronoStateEnd.hpp"

GameChrono::GameChrono( const int aDifficulty )
{  
  mDifficulty           = aDifficulty;
  loadDifficultyFile ( "difct_cfg.txt" );
  mMode = GameOptions::eModeChrono;
  mLapsedLaps = 0;
  mTimer = NULL;
  
}

void GameChrono::_initStates()
{
  mGameContext = new GameChronoContext( mCam, this);
}


GameChrono::~GameChrono()
{
  delete mTimer;
  delete mGameContext;
}

IoAbstract*  GameChrono::handlerIOinput() 
{
  return mGameContext->_stateHandlerIOinput();
}

void GameChrono::_setCamera()
{
  setCamera3rdPerson();
}

void GameChrono::_update( const float aLapsedTime )
{
  mGameContext->_stateUpdate(aLapsedTime);
}

void GameChrono::_updateCamera( const float aLapsedTime )
{
  updateCamera3rdPerson(aLapsedTime);

  //mGameContext->_stateUpdate(aLapsedTime);
}

void GameChrono::_initTrackDesc()
{
  mTrackDesc.totalLength  = mLoaded_trackLength;
  
  mTrackDesc.curvatureMin = 1;
  mTrackDesc.curvatureMin = 2;
  mTrackDesc.numUnitsPerCurve = 50;
  mTrackDesc.iterations = mLoaded_diff_Track_Num_Intersections[mDifficulty];

  mTrackDesc.usesIA = false;
  mTrackDesc.trackWidth = mLoaded_diff_Track_Width[mDifficulty];
  mTrackDesc.trackWidthEntities = mLoaded_diff_Track_Width_Entities[mDifficulty];
}

void GameChrono::_initGameState()
{
#ifndef GBP_GFXDEBUG_TRACK
      float time = mTrack->length() * 100.0f / 45 * mLoaded_diff_chrono_mult[mDifficulty];
#endif
#ifdef GBP_GFXDEBUG_TRACK
      float time = mTrack[0]->length() * 100.0f / 45 * mLoaded_diff_chrono_mult[mDifficulty];
#endif 
  
  if( mTimer != NULL )
    delete mTimer;
  mTimer = TrackTimer::timerUp( time );
  mTimer->stop();
}

void GameChrono::_initEntityDesc()
{
  mTrackEntDesc.archTimeUpUsed = false;
}

void GameChrono::_checkEndGame( const float aLapsedTime )
{
  Global::mLaps = mLapsedLaps;

  ////////////////////////////////////
  //  timer
  mTimer->updateTime( aLapsedTime );
  Global::gChronoDisplay = mTimer->displayString();
}

void GameChrono::_triggerEnd(Ball &aBall)
{
  *mLog->mFileEvents << "fin chrono " << std::endl;
  {
    mGameFinished = true;
    mGameWon = true;
    mGameContext->currentState( mGameContext->mStateEnd );    
    *mLog->mFileEvents << "fin chrono qweriouqweriouwe" << std::endl;
  }
}

std::string  GameChrono::_getMainMessage() const
{   
  return mGameContext->mCurrentState->_getMainMessage();
}


void GameChrono::_drawScene()
{ 
  mGameContext->_stateDrawScene();
}