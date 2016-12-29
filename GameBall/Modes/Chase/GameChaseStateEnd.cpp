#include  "GameBall\Modes\Chase\GameChaseStateEnd.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameChaseStateEnd::GameChaseStateEnd( GameChaseContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
}

GameChaseStateEnd::~GameChaseStateEnd()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameChaseStateEnd::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameChaseStateEnd::_stateUpdate( const float aLapsedTime )
{
}

void GameChaseStateEnd::_stateDrawScene()
{
}



std::string  GameChaseStateEnd::_getMainMessage() const
{
  std::string s;
  if ( mGame->isGameWon() )
    s = std::string ( "You won");
  else
    s = std::string ( "You lost");

  return s;
}