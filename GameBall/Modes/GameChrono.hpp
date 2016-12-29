#ifndef _GameBP_Chrono_HPP_
#define _GameBP_Chrono_HPP_


#include  "GameBall/Game.hpp"
#include  "GameBall\Modes\Chrono\GameChronoContext.hpp"

class GameChrono : public Game {
public:
  GameChrono( const int aDifficulty );  
  ~GameChrono();

  virtual IoAbstract* handlerIOinput();
  virtual void _setCamera();
  virtual void _updateCamera( const float aLapsedTime );
  virtual void _update( const float aLapsedTime );  
  virtual void _initTrackDesc();
  virtual void _initGameState(); 
  virtual void _initEntityDesc(); 
  virtual void _initBalls(){};
  virtual void _initStates();

  virtual void _drawScene();
  virtual void _drawBalls(){ drawBalls(); };
  virtual std::string  _getMainMessage() const;
  virtual void _IAstep( const float aLapsedTime ){};
  virtual void _checkEndGame( const float aLapsedTime );  
  virtual void _triggerEnd(Ball &aBall);
  virtual void _triggerAddTime ( const float aTime ){};
private:
  int     mLapsedLaps;
  const static int mNumLaps = 3;
  TrackTimer*           mTimer;

  GameChronoContext*             mGameContext;  

  friend class    GameChronoStateStart;
};


#endif  //_GameBP_Chrono_HPP_
