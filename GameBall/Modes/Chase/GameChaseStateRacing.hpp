#ifndef _GameBP_Chase_State_Racing_HPP_
#define _GameBP_Chase_State_Racing_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameChaseContext;

class GameChaseStateRacing : public GameState {
public:
  GameChaseStateRacing( GameChaseContext* aContext, Camera* aCam, Game* aGame );  
  ~GameChaseStateRacing();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;


private:
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  float                 mTime;
  GameChaseContext*     mGameContext;

};


#endif  //_GameBP_Chase_State_Racing_HPP_
