#include  "GameBall\Modes\Lab\GameLabStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameLabStateRacing::GameLabStateRacing( GameLabContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
}

GameLabStateRacing::~GameLabStateRacing()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameLabStateRacing::_stateHandlerIOinput()
{
  return mIOinput;
}

void GameLabStateRacing::_stateUpdate( const float aLapsedTime )
{
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {

  }
}

void GameLabStateRacing::_stateDrawScene()
{
}


std::string  GameLabStateRacing::_getMainMessage() const
{
  std::string s;//( "Run for your life");
  return s;
}