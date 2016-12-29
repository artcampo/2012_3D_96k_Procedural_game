#include  "GameBall\Modes\Chase\GameChaseContext.hpp"
#include  "GameBall\Modes\State\GameState.hpp"
#include  "GameBall\Modes\Chase\GameChaseStateStart.hpp"
#include  "GameBall\Modes\Chase\GameChaseStateRacing.hpp"
#include  "GameBall\Modes\Chase\GameChaseStateEnd.hpp"

GameChaseContext::GameChaseContext( Camera* aCam, Game* aGame )
  : GameContext( aCam, aGame )
{
  mStateStart   = new GameChaseStateStart   (this, aCam, aGame);
  mStateRacing  = new GameChaseStateRacing  (this, aCam, aGame);
  mStateEnd     = new GameChaseStateEnd     (this, aCam, aGame);
  mCurrentState = mStateStart;
}

GameChaseContext::~GameChaseContext()
{
  delete mStateStart;
  delete mStateRacing;
  delete mStateEnd;
}

IoAbstract* GameChaseContext::_stateHandlerIOinput()
{
  return mCurrentState->_stateHandlerIOinput();
}

void GameChaseContext::_stateUpdate( const float aLapsedTime )
{
  mCurrentState->_stateUpdate( aLapsedTime );
}

void GameChaseContext::_stateDrawScene()
{
  mCurrentState->_stateDrawScene();
}

void GameChaseContext::currentState( GameState* aState )
{
  mCurrentState = aState;
}
