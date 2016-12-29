#ifndef _GameBP_Context_HPP_
#define _GameBP_Context_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;

class GameContext {
public:

  GameContext( Camera* aCam, Game* aGame ){};  
  ~GameContext(){};

  virtual IoAbstract* _stateHandlerIOinput() = 0;
  virtual void _stateUpdate( const float aLapsedTime ) = 0;
  virtual void _stateDrawScene() = 0;
  virtual void currentState( GameState* aState ) = 0;

private:

};


#endif  //_GameBP_Context_HPP_
