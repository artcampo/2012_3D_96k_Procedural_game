#ifndef _GameBP_Recording_HPP_
#define _GameBP_Recording_HPP_

#include "dx_misc.hpp"
#include <vector>


class Game;

class GameRecording {
public:  
  GameRecording( Game* aGame );
  ~GameRecording(){};

public:
  void    store( const float aLapsedTime );
  
  void    play( const float aLapsedTime );

  D3DXVECTOR3     ballOrientation() const;
  D3DXVECTOR3     ballPosition() const;

private:
  Game* mGame;
  std::vector<float>          mTimePos;
  std::vector<D3DXVECTOR3>    mBallPos;  
  std::vector<D3DXVECTOR3>    mBallOrientation;

  int                         mPlayIndex;
  friend class GameReplay;
};


#endif  //_GameBP_Ball_HPP_
