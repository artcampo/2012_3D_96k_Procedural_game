#ifndef _GameBP_Spartacus_State_Start_HPP_
#define _GameBP_Spartacus_State_Start_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameSpartacusContext;

class GameSpartacusStateStart : public GameState {
public:
  GameSpartacusStateStart( GameSpartacusContext* aContext, Camera* aCam, Game* aGame );  
  ~GameSpartacusStateStart();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;
  
private:
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;  
  float                 mTime;
  GameSpartacusContext*     mGameContext;

};


#endif  //_GameBP_Spartacus_State_Start_HPP_
