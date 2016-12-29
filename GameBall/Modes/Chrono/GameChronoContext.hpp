#ifndef _GameBP_Chrono_Context_HPP_
#define _GameBP_Chrono_Context_HPP_

#include  "GameBall\Modes\State\GameContext.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameChrono;
class GameState;
class GameChronoStateStart;
class GameChronoStateRacing;
class GameChronoStateEnd;

class GameChronoContext : public GameContext {
public:
  GameChronoContext( Camera* aCam, GameChrono* aGame );  
  ~GameChronoContext();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual void currentState( GameState* aState );

private:
  GameState*            mCurrentState;
  GameChronoStateStart*  mStateStart;
  GameChronoStateRacing* mStateRacing;
  GameChronoStateEnd*    mStateEnd;
  
  friend class GameChronoStateStart;
  friend class GameChronoStateRacing;
  friend class GameChrono;


};


#endif  //_GameBP_Chrono_Context_HPP_
