#ifndef _GameBP_Chase_Context_HPP_
#define _GameBP_Chase_Context_HPP_

#include  "GameBall\Modes\State\GameContext.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameState;
class GameChaseStateStart;
class GameChaseStateRacing;
class GameChaseStateEnd;

class GameChaseContext : public GameContext {
public:
  GameChaseContext( Camera* aCam, Game* aGame );  
  ~GameChaseContext();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual void currentState( GameState* aState );

private:
  GameState*            mCurrentState;
  GameChaseStateStart*  mStateStart;
  GameChaseStateRacing* mStateRacing;
  GameChaseStateEnd*    mStateEnd;
  
  friend class GameChaseStateStart;
  friend class GameChaseStateRacing;
  friend class GameChase;


};


#endif  //_GameBP_Chase_Context_HPP_
