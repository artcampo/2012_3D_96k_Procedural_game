#include  "GameBall\Modes\Lab\GameLabContext.hpp"
#include  "GameBall\Modes\State\GameState.hpp"
#include  "GameBall\Modes\Lab\GameLabStateStart.hpp"
#include  "GameBall\Modes\Lab\GameLabStateRacing.hpp"
#include  "GameBall\Modes\Lab\GameLabStateEnd.hpp"
#include  "GameBall\Modes\GameLab.hpp"

GameLabContext::GameLabContext( Camera* aCam, GameLab* aGame )
  : GameContext( aCam, aGame )
{
  mStateStart   = new GameLabStateStart   (this, aCam, aGame);
  mStateRacing  = new GameLabStateRacing  (this, aCam, aGame);
  mStateEnd     = new GameLabStateEnd     (this, aCam, aGame);
  mCurrentState = mStateStart;
}

GameLabContext::~GameLabContext()
{
  delete mStateStart;
  delete mStateRacing;
  delete mStateEnd;
}

IoAbstract* GameLabContext::_stateHandlerIOinput()
{
  return mCurrentState->_stateHandlerIOinput();
}

void GameLabContext::_stateUpdate( const float aLapsedTime )
{
  mCurrentState->_stateUpdate( aLapsedTime );
}

void GameLabContext::_stateDrawScene()
{
  mCurrentState->_stateDrawScene();
}

void GameLabContext::currentState( GameState* aState )
{
  mCurrentState = aState;
}
