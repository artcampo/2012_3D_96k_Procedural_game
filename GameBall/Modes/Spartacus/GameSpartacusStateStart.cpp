#include  "GameBall\Modes\Spartacus\GameSpartacusStateStart.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusStateRacing.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusContext.hpp"

GameSpartacusStateStart::GameSpartacusStateStart( GameSpartacusContext* aContext, Camera* aCam, Game* aGame )
  : GameState(aCam, aGame),
    mTime(0)
{
  mIOinput        = new IoInput( aCam, aGame, aGame->mGameApp );  	
  mIOinputBlocked = new IoInputBlocked( aCam, aGame, aGame->mGameApp );  	
  mGameContext    = aContext;
}

GameSpartacusStateStart::~GameSpartacusStateStart()
{
  delete mIOinput;
  delete mIOinputBlocked;
}


IoAbstract* GameSpartacusStateStart::_stateHandlerIOinput()
{
  return mIOinputBlocked;
}

void GameSpartacusStateStart::_stateUpdate( const float aLapsedTime )
{
  mTime += aLapsedTime;
  if ( mTime > 3*100 )
  {
    mGameContext->currentState( mGameContext->mStateRacing );
  }
}

void GameSpartacusStateStart::_stateDrawScene()
{
}


std::string  GameSpartacusStateStart::_getMainMessage() const
{
  std::string s;
  if ( mTime >   0 ) s = std::string("1");
  if ( mTime > 100 ) s = std::string("2");
  if ( mTime > 200 ) s = std::string("3");
  return s;
}