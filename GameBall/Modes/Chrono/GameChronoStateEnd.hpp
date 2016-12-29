#ifndef _GameBP_Chrono_State_End_HPP_
#define _GameBP_Chrono_State_End_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameChronoContext;

class GameChronoStateEnd : public GameState {
public:
  GameChronoStateEnd( GameChronoContext* aContext, Camera* aCam, Game* aGame );  
  ~GameChronoStateEnd();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  

private:
  Game*                 mGame;
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  float                 mTime;
  GameChronoContext*     mGameContext;

};


#endif  //_GameBP_Chrono_State_End_HPP_
