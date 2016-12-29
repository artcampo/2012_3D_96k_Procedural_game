#ifndef _GameBP_State_HPP_
#define _GameBP_State_HPP_

#include <iostream>

class IoAbstract;
class IoInput;
class IoInputBlocked;
class Camera;
class Game;
class GameContext;

class GameState {
public:  
  
  GameState(  Camera* aCam, Game* aGame ){};  
  ~GameState(){};

  virtual IoAbstract* _stateHandlerIOinput() = 0;
  virtual void _stateUpdate( const float aLapsedTime ) = 0;
  virtual void _stateDrawScene() = 0;
  virtual std::string  _getMainMessage() const = 0;
  
private:  

};


#endif  //_GameBP_Chase_HPP_
