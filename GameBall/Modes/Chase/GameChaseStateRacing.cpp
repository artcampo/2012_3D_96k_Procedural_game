#include  "GameBall\Modes\Chase\GameChaseStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameChaseStateRacing::GameChaseStateRacing( GameChaseContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp);  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
}

GameChaseStateRacing::~GameChaseStateRacing()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameChaseStateRacing::_stateHandlerIOinput()
{
  return mIOinput;
}

void GameChaseStateRacing::_stateUpdate( const float aLapsedTime )
{
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {

  }
}

void GameChaseStateRacing::_stateDrawScene()
{
}


std::string  GameChaseStateRacing::_getMainMessage() const
{
  std::string s;//( "Run for your life");
  return s;
}