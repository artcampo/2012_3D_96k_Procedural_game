#ifndef _GameBP_Spartacus_State_End_HPP_
#define _GameBP_Spartacus_State_End_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameSpartacusContext;

class GameSpartacusStateEnd : public GameState {
public:
  GameSpartacusStateEnd( GameSpartacusContext* aContext, Camera* aCam, Game* aGame );  
  ~GameSpartacusStateEnd();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  

private:
  Game*                 mGame;
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  float                 mTime;
  GameSpartacusContext*     mGameContext;

};


#endif  //_GameBP_Spartacus_State_End_HPP_
