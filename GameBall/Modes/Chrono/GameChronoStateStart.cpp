#include  "GameBall\Modes\Chrono\GameChronoStateStart.hpp"
#include  "GameBall\Modes\Chrono\GameChronoStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"
#include  "GameBall\Modes\Chrono\GameChronoContext.hpp"
#include  "GameBall\Modes\GameChrono.hpp"

GameChronoStateStart::GameChronoStateStart( GameChronoContext* aContext, Camera* aCam, GameChrono* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
}

GameChronoStateStart::~GameChronoStateStart()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameChronoStateStart::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameChronoStateStart::_stateUpdate( const float aLapsedTime )
{
  mGame->mBall->mBallBody->standstill();
  mGame->mBall->inGameReset();
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {
    mGame->mTimer->resume();
    mGameContext->currentState( mGameContext->mStateRacing );
  }
}

void GameChronoStateStart::_stateDrawScene()
{
}


std::string  GameChronoStateStart::_getMainMessage() const
{
  std::string s;
  if ( mTime >   0 ) s = std::string("1");
  if ( mTime > 100 ) s = std::string("2");
  if ( mTime > 200 ) s = std::string("3");
  return s;
}