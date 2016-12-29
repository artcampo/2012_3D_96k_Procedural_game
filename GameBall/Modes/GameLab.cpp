#include  "GameBall/Modes/GameLab.hpp"
#include  "Project_Dependant/IOAbstract.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall\Modes\Lab\GameLabStateEnd.hpp"

GameLab::GameLab( const int aDifficulty )
{ 
  loadDifficultyFile ( "difct_cfg.txt" );
  mDifficulty           = aDifficulty;
  mMode = GameOptions::eModeLab;
  mTimer = NULL;  
}

void GameLab::_initStates()
{
  mGameContext = new GameLabContext( mCam, this);
}

GameLab::~GameLab()
{
  delete mTimer;
  delete mGameContext;
}

IoAbstract*  GameLab::handlerIOinput()
{
  return mGameContext->_stateHandlerIOinput();
}

void GameLab::_setCamera()
{
  setCamera3rdPerson();

  
}

void GameLab::_update( const float aLapsedTime )
{
  mGameContext->_stateUpdate(aLapsedTime);
}

void GameLab::_updateCamera( const float aLapsedTime )
{
  updateCamera3rdPerson( aLapsedTime );
  //mGameContext->_stateUpdate(aLapsedTime);
}

void GameLab::_initTrackDesc()
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

void GameLab::_initGameState()
{
  *mLog->mFileMenu << "Timer down added." << std::endl;
  if( mTimer != NULL )
    delete mTimer;
  mTimer = TrackTimer::timerDown( 15*100.0f );
  mTimer->stop();
}

void GameLab::_initEntityDesc()
{
  mTrackEntDesc.archTimeUpUsed = true;
  mTrackEntDesc.archDensity = 60;
  mTrackEntDesc.archDensity /= mLoaded_diff_lab_mult[mDifficulty];
}

void GameLab::_checkEndGame( const float aLapsedTime )
{
  if ( mGameFinished )
    return;
  
  ////////////////////////////////////
  //  timer
  mTimer->updateTime( aLapsedTime );
  Global::gChronoDisplay = mTimer->displayString();

  mGameFinished = mTimer->isTimeFinished();
  *mLog->mFileMenu << "Timer:"<<mTimer->displayString() << std::endl;

}

void GameLab::_triggerEnd(Ball &aBall)
{
  mGameFinished = true;  
  *mLog->mFileEvents << "Finished lab" <<std::endl;
}

void GameLab::_triggerAddTime ( const float aTime )
{
  mTimer->addTime(aTime);
}

std::string  GameLab::_getMainMessage() const
{
  return mGameContext->mCurrentState->_getMainMessage();
}


void GameLab::_drawScene()
{ 
  mGameContext->_stateDrawScene();
}