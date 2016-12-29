#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Lane.hpp"
#include  "Geometry/DxMathHelpers.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"
#include  "GameBall/Track/TrackPath.hpp"

using namespace TricksScore;

enum states{
  inLand,  
  inAir
};

Trick_Lane::Trick_Lane( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(inLand)
{  
}

/*
Trick_Lane::~Trick_Lane()
{ }
*/
  
void  Trick_Lane::update( const double        aLapsedTime, 
                            const bool          aHasStartedJump,
                            const bool          aHasLanded,    
                            const TrackPath*    aCurrentTrackPath)
{
  
  switch ( mState )
  {
  case inLand:
    if ( aHasStartedJump )
    {      
      mTrackPathAtJump  = aCurrentTrackPath;
      mState            = inAir;      
    }
    break;

  case inAir:
    if ( aHasLanded )
    {
     
      bool found = false;
      //  current one is not previous, nor any of its descendants
      found = aCurrentTrackPath == mTrackPathAtJump;
      for ( int i = 0; i < mTrackPathAtJump->numDescendants() ; ++i )
      {
        found |= aCurrentTrackPath == mTrackPathAtJump->descendant(i);
        TrackPath* p = mTrackPathAtJump->descendant(i);
        for ( int j = 0; j < p->numDescendants() ; ++j )
        {
          found |= aCurrentTrackPath == p->descendant(j);
          TrackPath* p2 = p->descendant(j);
          for ( int k = 0; k < p2->numDescendants() ; ++k )
            found |= aCurrentTrackPath == p2->descendant(k);
        }
      }
      // when going back wards it gets to parents
      for ( int i = 0; i < mTrackPathAtJump->numParents() ; ++i )
        found |= aCurrentTrackPath == mTrackPathAtJump->parent(i);
      

      if ( !found )              
      {
        
        mTrickManager->addPunctuation(  Score_Lane::mPoints +
        Score_Lane::mPoints/10*mNum,
                                        std::string("Lane to lane") );    

        //mBallVelPrev = aBallVel;
        ++mNum;
      }
      mState = inLand;
    }
    break;

  default: break;
  }
}


void  Trick_Lane::reset()
{
  mState = inLand;
  mNum = 0;
}