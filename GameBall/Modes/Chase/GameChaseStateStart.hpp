#ifndef _GameBP_Chase_State_Start_HPP_
#define _GameBP_Chase_State_Start_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameChaseContext;

class GameChaseStateStart : public GameState {
public:
  GameChaseStateStart( GameChaseContext* aContext, Camera* aCam, Game* aGame );  
  ~GameChaseStateStart();

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


#endif  //_GameBP_Chase_State_Start_HPP_
