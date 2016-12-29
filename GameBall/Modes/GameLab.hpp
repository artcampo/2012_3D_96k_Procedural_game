#ifndef _GameBP_Lab_HPP_
#define _GameBP_Lab_HPP_


#include  "GameBall/Game.hpp"
#include  "GameBall\Modes\Lab\GameLabContext.hpp"

class GameLabStateStart;

class GameLab : public Game {
public:
  GameLab( const int aDifficulty );
  ~GameLab();

  virtual IoAbstract* handlerIOinput();
  virtual void _setCamera();
  virtual void _update( const float aLapsedTime );
  virtual void _updateCamera( const float aLapsedTime );
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
  virtual void _triggerAddTime ( const float aTime );
private:
  TrackTimer*           mTimer;
  GameLabContext*             mGameContext;  

  friend class GameLabStateStart;
};


#endif  //_GameBP_Lab_HPP_
