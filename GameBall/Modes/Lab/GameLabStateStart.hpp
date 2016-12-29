#ifndef _GameBP_Lab_State_Start_HPP_
#define _GameBP_Lab_State_Start_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameLab;
class GameLabContext;

class GameLabStateStart : public GameState {
public:
  GameLabStateStart( GameLabContext* aContext, Camera* aCam, GameLab* aGame );  
  ~GameLabStateStart();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  
private:
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;  
  float                 mTime;
  GameLabContext*     mGameContext;
  GameLab*            mGame;

};


#endif  //_GameBP_Lab_State_Start_HPP_
