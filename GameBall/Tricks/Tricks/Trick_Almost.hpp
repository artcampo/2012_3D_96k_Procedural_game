#ifndef _Trick_Almost_HPP_
#define _Trick_Almost_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;
class TrackEdge;

class Trick_Almost{
public:
  //~Trick_Almost();
  
  Trick_Almost( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                                const bool          aHasStartedJump,
                                const bool          aHasLanded,  
                const float   aDistanceToCenterOfTrack,
                const float   aTrackWidth,
                const TrackEdge*    aCurrentTrackEdge);

public:
  int       mNum;
private:  
  Tricks*     mTrickManager;
  int         mState;  
  float       mCurrentTime;

  const static float mTime;

  
  bool  isOutside(  const float   aDistanceToCenterOfTrack,
                              const float   aTrackWidth );

};

#endif  //_Trick_Almost_HPP_
