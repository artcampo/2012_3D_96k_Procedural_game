#ifndef _GameBP_Spartacus_State_Racing_HPP_
#define _GameBP_Spartacus_State_Racing_HPP_

#include  "GameBall\Modes\State\GameState.hpp"

class IoInput;
class IoAbstract;
class IoInputBlocked;
class Camera;
class Game;
class GameSpartacus;
class GameSpartacusContext;

class GameSpartacusStateRacing : public GameState {
public:
  GameSpartacusStateRacing( GameSpartacusContext* aContext, Camera* aCam, GameSpartacus* aGame );  
  ~GameSpartacusStateRacing();

  virtual IoAbstract* _stateHandlerIOinput();
  virtual void _stateUpdate( const float aLapsedTime );
  virtual void _stateDrawScene();
  virtual std::string  _getMainMessage() const;


private:
  IoInput*              mIOinput;
  IoInputBlocked*       mIOinputBlocked;
  GameSpartacusContext* mGameContext;
  GameSpartacus*        mGame;

  float                 mTime;

  //  Display balls left
  bool                  mDisplayNumBalls;
  float                 mDisplayNumBallsTime;
  int                   mBallsInRace;



};


#endif  //_GameBP_Spartacus_State_Racing_HPP_
