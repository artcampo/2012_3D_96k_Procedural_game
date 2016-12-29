#include  "GameBall/Modes/GameSpartacus.hpp"
#include  "GameBall\Modes\State\GameState.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusContext.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusStateStart.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusStateRacing.hpp"
#include  "GameBall\Modes\Spartacus\GameSpartacusStateEnd.hpp"

GameSpartacusContext::GameSpartacusContext( Camera* aCam, GameSpartacus* aGame )
  : GameContext( aCam, aGame )
{
  mStateStart   = new GameSpartacusStateStart   (this, aCam, aGame);
  mStateRacing  = new GameSpartacusStateRacing  (this, aCam, aGame);
  mStateEnd     = new GameSpartacusStateEnd     (this, aCam, aGame);
  mCurrentState = mStateStart;
}

GameSpartacusContext::~GameSpartacusContext()
{
  delete mStateStart;
  delete mStateRacing;
  delete mStateEnd;
}

IoAbstract* GameSpartacusContext::_stateHandlerIOinput()
{
  return mCurrentState->_stateHandlerIOinput();
}

void GameSpartacusContext::_stateUpdate( const float aLapsedTime )
{
  mCurrentState->_stateUpdate( aLapsedTime );
}

void GameSpartacusContext::_stateDrawScene()
{
  mCurrentState->_stateDrawScene();
}

void GameSpartacusContext::currentState( GameState* aState )
{
  mCurrentState = aState;
}
