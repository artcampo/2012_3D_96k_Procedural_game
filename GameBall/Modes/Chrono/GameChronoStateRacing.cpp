#include  "GameBall\Modes\Chrono\GameChronoStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameChronoStateRacing::GameChronoStateRacing( GameChronoContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
}

GameChronoStateRacing::~GameChronoStateRacing()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameChronoStateRacing::_stateHandlerIOinput()
{
  return mIOinput;
}

void GameChronoStateRacing::_stateUpdate( const float aLapsedTime )
{  
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {

  }
}

void GameChronoStateRacing::_stateDrawScene()
{
}


std::string  GameChronoStateRacing::_getMainMessage() const
{
  std::string s;//( "Run for your life");
  return s;
}