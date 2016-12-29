#ifndef _GameBP_Chase_State_End_HPP_
#define _GameBP_Chase_State_End_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameChaseContext;

class GameChaseStateEnd : public GameState {
public:
  GameChaseStateEnd( GameChaseContext* aContext, Camera* aCam, Game* aGame );  
  ~GameChaseStateEnd();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  

private:
  Game*                 mGame;
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  float                 mTime;
  GameChaseContext*     mGameContext;

};


#endif  //_GameBP_Chase_State_End_HPP_
