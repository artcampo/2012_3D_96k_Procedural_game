#include  "GameBall\Modes\Chrono\GameChronoStateEnd.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"

GameChronoStateEnd::GameChronoStateEnd( GameChronoContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
}

GameChronoStateEnd::~GameChronoStateEnd()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameChronoStateEnd::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameChronoStateEnd::_stateUpdate( const float aLapsedTime )
{
}

void GameChronoStateEnd::_stateDrawScene()
{
}



std::string  GameChronoStateEnd::_getMainMessage() const
{
  std::string s;
  if ( mGame->isGameWon() )
    s = std::string ( "You won wer");
  else
    s = std::string ( "You lost");

  return s;
}