#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Grind.hpp"
#include  "Geometry/DxMathHelpers.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"
#include  "GameBall/Track/TrackEdge.hpp"
#include  "Global.hpp"

using namespace TricksScore;

const float Trick_Grind::mGrindTime    = 0.5*100.0f;


enum states{
  inLand,  
  grindStarted
};

Trick_Grind::Trick_Grind( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(inLand)
{  
}

/*
Trick_Grind::~Trick_Grind()
{ }
*/
  
void  Trick_Grind::update( const double        aLapsedTime, 
                            const bool          aIsInLand,  
                            const float   aDistanceToCenterOfTrack,
                            const float   aTrackWidth,
                            const TrackEdge*    aCurrentTrackEdge)
{
  Global::mExcludeEdge = int(aDistanceToCenterOfTrack);
 
  *mTrickManager->mLog->mFileEvents     
    << "Grind (" 
    << aDistanceToCenterOfTrack
    << "):"<< aTrackWidth*0.05f
    << " t: "<< mGrindCurrentTime
    << " of "<< mGrindTime;
  if ( isInGrind(aDistanceToCenterOfTrack, aTrackWidth ) )
    *mTrickManager->mLog->mFileEvents << " in grind";
  *mTrickManager->mLog->mFileEvents     
    << std::endl;
  switch ( mState )
  {
  case inLand:
    if (  isInGrind( aDistanceToCenterOfTrack, aTrackWidth ) )
    {  
      mGrindCurrentTime = 0.0f;
      mState            = grindStarted;      
    }
    
    break;

  case grindStarted:    
    if (  isInGrind(aDistanceToCenterOfTrack, aTrackWidth ) )
    { 
      mGrindCurrentTime += aLapsedTime;
      *mTrickManager->mLog->mFileEvents << "MADE IT-------------------------------------------------------------------------------------------"<< std::endl;
      if ( mGrindCurrentTime >= mGrindTime )
      {
        mTrickManager->addPunctuation(  Score_Grind::mPoints +
                                        Score_Grind::mPoints/10*mNum,
                                        std::string("Grind") );    
        mState = inLand;
        ++mNum;
      }
    }
    else
      mState = inLand;
    break;

  default: break;
  }
  
}

bool  Trick_Grind::isInGrind( const float   aDistanceToCenterOfTrack,
                              const float   aTrackWidth)
{
  return DxMath::equality( aTrackWidth, aDistanceToCenterOfTrack, aTrackWidth*0.1 );
}

void  Trick_Grind::reset()
{
  mState = inLand;
  mNum = 0;
}