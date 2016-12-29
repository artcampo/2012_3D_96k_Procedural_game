#include  "GameBall\Modes\Lab\GameLabStateEnd.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameLabStateEnd::GameLabStateEnd( GameLabContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
}

GameLabStateEnd::~GameLabStateEnd()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameLabStateEnd::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameLabStateEnd::_stateUpdate( const float aLapsedTime )
{
}

void GameLabStateEnd::_stateDrawScene()
{
}



std::string  GameLabStateEnd::_getMainMessage() const
{
  std::string s;
  if ( mGame->isGameWon() )
    s = std::string ( "You won");
  else
    s = std::string ( "You lost");

  return s;
}