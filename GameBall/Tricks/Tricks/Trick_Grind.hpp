#ifndef _Trick_Grind_HPP_
#define _Trick_Grind_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;
class TrackEdge;

class Trick_Grind{
public:
  //~Trick_Grind();
  
  Trick_Grind( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                const bool          aIsInLand,  
                const float   aDistanceToCenterOfTrack,
                const float   aTrackWidth,
                const TrackEdge*    aCurrentTrackEdge);

public:
  int       mNum;
private:  
  Tricks*     mTrickManager;
  int         mState;  
  float       mGrindCurrentTime;

  const static float mGrindTime;

  
  bool  isInGrind(  const float   aDistanceToCenterOfTrack,
                              const float   aTrackWidth );

};

#endif  //_Trick_Grind_HPP_
