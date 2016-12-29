#ifndef _GameBP_Replay_HPP_
#define _GameBP_Replay_HPP_

#include "dx_misc.hpp"
#include <vector>
#include  "Misc/Log.hpp"

class Game;
class Camera3rdPersonFly;

class GameReplay {
public:  
  GameReplay( Game* aGame );
  ~GameReplay(){delete mLog;};

public:   
  void    play( const float aLapsedTime );

private:
  Camera3rdPersonFly*    mCam;
  Game*   mGame;
  float   mCurrentTime;

public:
  float   mTime0;
  float   mTime1;
  Log*    mLog;
};


#endif  //_GameBP_Replay_HPP_
