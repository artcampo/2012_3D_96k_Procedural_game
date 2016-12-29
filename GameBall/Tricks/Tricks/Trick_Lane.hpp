#ifndef _Trick_Lane_HPP_
#define _Trick_Lane_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;
class TrackPath;

class Trick_Lane{
public:
  //~Trick_Lane();
  
  Trick_Lane( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                const bool          aHasStartedJump,
                const bool          aHasLanded,    
                const TrackPath*    aCurrentTrackPath);

public:
  int       mNum;
private:  
  Tricks*     mTrickManager;
  int         mState;  
  const TrackPath*  mTrackPathAtJump;
  


};

#endif  //_Trick_Lane_HPP_
