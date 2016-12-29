#include  "GameBall\Modes\Spartacus\GameSpartacusStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"
#include  "GameBall/Modes/GameSpartacus.hpp"
#include <sstream>

GameSpartacusStateRacing::GameSpartacusStateRacing( GameSpartacusContext* aContext, Camera* aCam, GameSpartacus* aGame )
  : GameState(aCam, aGame),
    mTime(0),
    mDisplayNumBalls(false)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
  mBallsInRace    = mGame->numBallsInRace();
}

GameSpartacusStateRacing::~GameSpartacusStateRacing()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameSpartacusStateRacing::_stateHandlerIOinput()
{
  return mIOinput;
}

void GameSpartacusStateRacing::_stateUpdate( const float aLapsedTime )
{
  mTime += aLapsedTime;
  if ( mBallsInRace != mGame->numBallsInRace() )
  {
    mBallsInRace          = mGame->numBallsInRace();
    mDisplayNumBalls      = true;
    mDisplayNumBallsTime  = 0;
  }
  else
  {
    if ( mDisplayNumBalls )
    {
      mDisplayNumBallsTime += aLapsedTime;
      if ( mDisplayNumBallsTime > 3*100.0f )
        mDisplayNumBalls = false;
    }
  }
}

void GameSpartacusStateRacing::_stateDrawScene()
{
}


std::string  GameSpartacusStateRacing::_getMainMessage() const
{
  std::string s;
  if ( mDisplayNumBalls )
  {
    if ( mBallsInRace == 1)
      s = std::string( "One ball left");
    else
    {
      std::stringstream out;
      out << mBallsInRace;
      s = out.str() + std::string( "balls left");
    }
  }
  return s;
}