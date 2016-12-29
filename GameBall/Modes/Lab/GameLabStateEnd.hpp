#ifndef _GameBP_Lab_State_End_HPP_
#define _GameBP_Lab_State_End_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameLabContext;

class GameLabStateEnd : public GameState {
public:
  GameLabStateEnd( GameLabContext* aContext, Camera* aCam, Game* aGame );  
  ~GameLabStateEnd();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  

private:
  Game*                 mGame;
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  float                 mTime;
  GameLabContext*     mGameContext;

};


#endif  //_GameBP_Lab_State_End_HPP_
