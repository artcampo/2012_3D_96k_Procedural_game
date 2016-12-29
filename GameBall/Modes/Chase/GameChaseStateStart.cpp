#include  "GameBall\Modes\Chase\GameChaseStateStart.hpp"
#include  "GameBall\Modes\Chase\GameChaseStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"
#include  "GameBall\Modes\Chase\GameChaseContext.hpp"

GameChaseStateStart::GameChaseStateStart( GameChaseContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
}

GameChaseStateStart::~GameChaseStateStart()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameChaseStateStart::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameChaseStateStart::_stateUpdate( const float aLapsedTime )
{
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {
    //GameState* s = mGameContext->mStateRacing->getInstance();
    //GameState* s = mGameContext->mStateRacing;
    //mGameContext->currentState( s );
    mGameContext->currentState( mGameContext->mStateRacing );
  }
}

void GameChaseStateStart::_stateDrawScene()
{
}


std::string  GameChaseStateStart::_getMainMessage() const
{
  std::string s;
  if ( mTime >   0 ) s = std::string("1");
  if ( mTime > 100 ) s = std::string("2");
  if ( mTime > 200 ) s = std::string("3");
  return s;
}