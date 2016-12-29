#include  "GameBall\Modes\Lab\GameLabStateStart.hpp"
#include  "GameBall\Modes\Lab\GameLabStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"
#include  "GameBall\Modes\Lab\GameLabContext.hpp"
#include  "GameBall\Modes\GameLab.hpp"

GameLabStateStart::GameLabStateStart( GameLabContext* aContext, Camera* aCam, GameLab* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
  mGame           = aGame;
}

GameLabStateStart::~GameLabStateStart()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameLabStateStart::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameLabStateStart::_stateUpdate( const float aLapsedTime )
{
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {
    //GameState* s = mGameContext->mStateRacing->getInstance();
    //GameState* s = mGameContext->mStateRacing;
    //mGameContext->currentState( s );
    mGame->mTimer->resume();
    mGameContext->currentState( mGameContext->mStateRacing );
  }
}

void GameLabStateStart::_stateDrawScene()
{
}


std::string  GameLabStateStart::_getMainMessage() const
{
  std::string s;
  if ( mTime >   0 ) s = std::string("1");
  if ( mTime > 100 ) s = std::string("2");
  if ( mTime > 200 ) s = std::string("3");
  return s;
}