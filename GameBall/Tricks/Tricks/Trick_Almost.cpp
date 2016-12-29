#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Almost.hpp"
#include  "Geometry/DxMathHelpers.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"
#include  "GameBall/Track/TrackEdge.hpp"
#include  "Global.hpp"

using namespace TricksScore;

const float Trick_Almost::mTime    = 0.5*100.0f;


enum states{
  inLand,  
  inAir,
  inAirWaiting
};

Trick_Almost::Trick_Almost( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(inLand)
{  
}

/*
Trick_Almost::~Trick_Almost()
{ }
*/
  
void  Trick_Almost::update( const double        aLapsedTime, 
                            const bool          aHasStartedJump,
                            const bool          aHasLanded,   
                            const float   aDistanceToCenterOfTrack,
                            const float   aTrackWidth,
                            const TrackEdge*    aCurrentTrackEdge)
{
  Global::mExcludeEdge = int(aDistanceToCenterOfTrack);
 

  switch ( mState )
  {
  case inLand:
    if ( aHasStartedJump )
    {  
      mCurrentTime      = 0;
      mState            = inAir;      
    }    
    break;

  case inAir:    
    if (  isOutside(aDistanceToCenterOfTrack, aTrackWidth ) )
    { 
      mCurrentTime += aLapsedTime;
      
      if ( mCurrentTime >= mTime )
      {
        mTrickManager->addPunctuation(  Score_Almost::mPoints +
                                        Score_Almost::mPoints/10*mNum,
                                        std::string("Almost off") );    
        mState = inAirWaiting;
        ++mNum;
      }
    }    
    if ( aHasLanded )
      mState = inLand;
    break;

  case inAirWaiting:
    if ( aHasLanded )
      mState = inLand;
    break;

  default: break;
  }
  
}

bool  Trick_Almost::isOutside( const float   aDistanceToCenterOfTrack,
                              const float   aTrackWidth)
{
  return DxMath::greaterThan( aDistanceToCenterOfTrack, aTrackWidth, aTrackWidth*0.1 );
}

void  Trick_Almost::reset()
{
  mState = inLand;
  mNum = 0;
}