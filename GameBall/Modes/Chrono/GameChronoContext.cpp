#include  "GameBall\Modes\Chrono\GameChronoContext.hpp"
#include  "GameBall\Modes\State\GameState.hpp"
#include  "GameBall\Modes\Chrono\GameChronoStateStart.hpp"
#include  "GameBall\Modes\Chrono\GameChronoStateRacing.hpp"
#include  "GameBall\Modes\Chrono\GameChronoStateEnd.hpp"
#include  "GameBall\Modes\GameChrono.hpp"

GameChronoContext::GameChronoContext( Camera* aCam, GameChrono* aGame )
  : GameContext( aCam, aGame )
{
  mStateStart   = new GameChronoStateStart   (this, aCam, aGame);
  mStateRacing  = new GameChronoStateRacing  (this, aCam, aGame);
  mStateEnd     = new GameChronoStateEnd     (this, aCam, aGame);
  mCurrentState = mStateStart;
}

GameChronoContext::~GameChronoContext()
{
  delete mStateStart;
  delete mStateRacing;
  delete mStateEnd;
}

IoAbstract* GameChronoContext::_stateHandlerIOinput()
{
  return mCurrentState->_stateHandlerIOinput();
}

void GameChronoContext::_stateUpdate( const float aLapsedTime )
{
  mCurrentState->_stateUpdate( aLapsedTime );
}

void GameChronoContext::_stateDrawScene()
{
  mCurrentState->_stateDrawScene();
}

void GameChronoContext::currentState( GameState* aState )
{
  mCurrentState = aState;
}
