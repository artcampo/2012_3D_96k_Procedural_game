#ifndef _GameBP_Spartacus_Context_HPP_
#define _GameBP_Spartacus_Context_HPP_

#include  "GameBall\Modes\State\GameContext.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameSpartacus;
class GameState;
class GameSpartacusStateStart;
class GameSpartacusStateRacing;
class GameSpartacusStateEnd;

class GameSpartacusContext : public GameContext {
public:
  GameSpartacusContext( Camera* aCam, GameSpartacus* aGame );  
  ~GameSpartacusContext();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual void currentState( GameState* aState );

private:
  GameState*            mCurrentState;
  GameSpartacusStateStart*  mStateStart;
  GameSpartacusStateRacing* mStateRacing;
  GameSpartacusStateEnd*    mStateEnd;
  
  friend class GameSpartacusStateStart;
  friend class GameSpartacusStateRacing;
  friend class GameSpartacus;


};


#endif  //_GameBP_Spartacus_Context_HPP_
