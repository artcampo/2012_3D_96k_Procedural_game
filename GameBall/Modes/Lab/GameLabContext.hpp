#ifndef _GameBP_Lab_Context_HPP_
#define _GameBP_Lab_Context_HPP_

#include  "GameBall\Modes\State\GameContext.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameLab;
class GameState;
class GameLabStateStart;
class GameLabStateRacing;
class GameLabStateEnd;

class GameLabContext : public GameContext {
public:
  GameLabContext( Camera* aCam, GameLab* aGame );  
  ~GameLabContext();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual void currentState( GameState* aState );

private:
  GameState*            mCurrentState;
  GameLabStateStart*  mStateStart;
  GameLabStateRacing* mStateRacing;
  GameLabStateEnd*    mStateEnd;
  
  friend class GameLabStateStart;
  friend class GameLabStateRacing;
  friend class GameLab;


};


#endif  //_GameBP_Lab_Context_HPP_
