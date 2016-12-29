#ifndef _GameBP_Chrono_State_Racing_HPP_
#define _GameBP_Chrono_State_Racing_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameChronoContext;

class GameChronoStateRacing : public GameState {
public:
  GameChronoStateRacing( GameChronoContext* aContext, Camera* aCam, Game* aGame );  
  ~GameChronoStateRacing();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;


private:
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  float                 mTime;
  GameChronoContext*    mGameContext;

};


#endif  //_GameBP_Chrono_State_Racing_HPP_
