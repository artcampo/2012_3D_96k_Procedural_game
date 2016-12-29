#ifndef _GameBP_Chrono_State_Start_HPP_
#define _GameBP_Chrono_State_Start_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameChrono;
class GameChronoContext;

class GameChronoStateStart : public GameState {
public:
  GameChronoStateStart( GameChronoContext* aContext, Camera* aCam, GameChrono* aGame );  
  ~GameChronoStateStart();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  
private:
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;  
  float                 mTime;
  GameChronoContext*     mGameContext;

  GameChrono*           mGame;

};


#endif  //_GameBP_Chrono_State_Start_HPP_
