#include  "GameBall\Modes\Spartacus\GameSpartacusStateEnd.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameSpartacusStateEnd::GameSpartacusStateEnd( GameSpartacusContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
}

GameSpartacusStateEnd::~GameSpartacusStateEnd()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameSpartacusStateEnd::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameSpartacusStateEnd::_stateUpdate( const float aLapsedTime )
{
}

void GameSpartacusStateEnd::_stateDrawScene()
{
}



std::string  GameSpartacusStateEnd::_getMainMessage() const
{
  std::string s;
  if ( mGame->isGameWon() )
    s = std::string ( "You won");
  else
    s = std::string ( "You lost");

  return s;
}