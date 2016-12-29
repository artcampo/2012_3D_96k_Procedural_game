#include <vector>
#include <string>
#include <sstream>

#include  "Misc/Log.hpp"
#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_AirTurbo.hpp"
#include  "GameBall/Tricks/Tricks/Trick_GettingHigh.hpp"
#include  "GameBall/Tricks/Tricks/Trick_BigFall.hpp"
#include  "GameBall/Tricks/Tricks/Trick_MoodSwing.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Ponder.hpp"
#include  "GameBall/Tricks/Tricks/Trick_OverBumper.hpp"
#include  "GameBall/Tricks/Tricks/Trick_SwingerLife.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Lane.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Grind.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Almost.hpp"



const float Tricks::mComboMultiplier = 1.2f;
const float Tricks::mMaxTimeCombo    = 6*100.0f;

enum states{
  ready,
  inLand,  
  inAir,
  inAirFalling
};

Tricks::Tricks( )
: mInCombo( false ),
  mComboPoints(0),
  mTotalPoints(0),
  mState(ready),
  mStateLandedJump(false),
  mStateJumpFalling(false),
  mStateStartedJump(false),
  mStateInLand(false),
  mMaxComboPoints(0),
  mMaxComboStartTime(0),
  mMaxComboEndTime(1)
{
  mTrickAirTurbo    = new Trick_AirTurbo    ( this );
  mTrickHigh        = new Trick_GettingHigh ( this );
  mTrickFall        = new Trick_BigFall     ( this );
  mTrickMood        = new Trick_MoodSwing   ( this );
  mTrickPonder      = new Trick_Ponder      ( this );
  mTrickOverBumper  = new Trick_OverBumper  ( this );
  mTrickSwinger     = new Trick_SwingerLife ( this );
  mTrickLane        = new Trick_Lane        ( this );
  mTrickGrind       = new Trick_Grind       ( this );
  mTrickAlmost      = new Trick_Almost      ( this );
  mLog              = new Log(); 
}


Tricks::~Tricks()
{
  delete mLog;
}


void  Tricks::update( const float  aLapsedTime, 
                     const float  aGameTime, 
            const bool    aIsInContact,
            const bool    aHadContact,
            const bool    aUsedNitro,
            const TrackPath*    aCurrentTrackPath,
            const TrackEdge*    aCurrentTrackEdge,
            const float   aDistanceToCenterOfTrack,
            const float   aTrackWidth,
            const Ball&   aBall )
{
  mGameCurrentTime = aGameTime;
  if ( mInCombo )
  {
    mLapsedTimeCombo += aLapsedTime;
    if ( mLapsedTimeCombo >= mMaxTimeCombo )
    {
      endCombo();
    }
  }

  stateMachine( aLapsedTime, aIsInContact, aHadContact, aUsedNitro, aBall );

  mTrickMood->update    ( aLapsedTime, mStateStartedJump, mStateLandedJump, aBall.mBallBody->getvWS() );
  mTrickPonder->update  ( aLapsedTime, mStateStartedJump, mStateLandedJump, aBall.mBallBody->getvWS() );
  mTrickFall->update    ( aLapsedTime, mStateStartedJump, mStateLandedJump, mStateJumpAbsHeight );
  
  mTrickHigh->update    ( aLapsedTime, 
                          mStateStartedJump, 
                          mStateLandedJump, 
                          mStateJumpFalling, 
                          mStateJumpMaxHeight );
  mTrickAirTurbo->update( aLapsedTime, 
                          mStateStartedJump, 
                          mStateLandedJump,
                          aUsedNitro, 
                          mStateJumpMaxHeight, 
                          aBall.mBallBody->getvWS() );
  

  mTrickOverBumper->update( mColBumperOver.size() > 0 );
  mTrickSwinger->update( mColBumperLeft, mColBumperRight );
  
  mTrickLane->update( aLapsedTime, mStateStartedJump, mStateLandedJump, aCurrentTrackPath );
  mTrickGrind->update( aLapsedTime, mStateInLand, aDistanceToCenterOfTrack, aTrackWidth, aCurrentTrackEdge );
  mTrickAlmost->update( aLapsedTime, mStateStartedJump, mStateLandedJump, aDistanceToCenterOfTrack, aTrackWidth, aCurrentTrackEdge );
}


std::string Tricks::displayText() const
{
  return mMessage;
}

int Tricks::punctuation() const
{
  return int(mTotalPoints);
}

void    Tricks::addPunctuation( const int aPoints, const std::string& aTrickName )
{
  if ( ! mInCombo )
    mComboStartTime = mGameCurrentTime;
  mComboPoints      += aPoints;
  mInCombo          = true;
  mLapsedTimeCombo  = 0.0f;
  mTricksInCombo.push_back( aTrickName );
  prepareMessage();
}

void    Tricks::endCombo()
{
  if ( mComboPoints > mMaxComboPoints )
  {
    mMaxComboPoints     = mComboPoints;
    mMaxComboStartTime  = mComboStartTime;
    mMaxComboEndTime    = mGameCurrentTime;
  }

  mTotalPoints += mComboPoints;
  mInCombo = 0;
  mComboPoints = 0;
  mTricksInCombo.clear();
  prepareMessage();

  mTrickAirTurbo->mNum    = 0;
  mTrickHigh->mNum        = 0;
  mTrickFall->mNum        = 0;
  mTrickMood->mNum        = 0;
  mTrickPonder->mNum      = 0;
  mTrickOverBumper->mNum  = 0;
  mTrickSwinger->mNum     = 0;
  mTrickLane->mNum        = 0;
  mTrickGrind->mNum       = 0;
  mTrickAlmost->mNum      = 0;
}


void    Tricks::prepareMessage()
{
  mMessage = std::string("");

  int totalLettersWithoutNewLine = 0;
  for ( int i = 0; i < mTricksInCombo.size() ; ++i) 
  {
    mMessage += mTricksInCombo[i];

    totalLettersWithoutNewLine += mTricksInCombo[i].size();
    if ( totalLettersWithoutNewLine > 40 )
    {
      totalLettersWithoutNewLine = 0;
      mMessage += std::string("\n");
    }

    if ( i != (mTricksInCombo.size() - 1 ) )
      mMessage += std::string(" + ");

    if ( mMessage.size() > 150 )
      mMessage = std::string("... + ");
  }

  if ( mInCombo )
  {
    std::stringstream ss;
    ss << mComboPoints;
    mMessage += std::string(": ") + ss.str(); 
  }

}


void  Tricks::reset()
{
  mState = ready;
  mTrickAirTurbo->reset();
  mTrickHigh->reset();
  mTrickFall->reset();
  mTrickMood->reset();
  mTrickPonder->reset();
  mTrickOverBumper->reset();
  mTrickSwinger->reset();
  mTrickLane->reset();
  mTrickGrind->reset();
  mTrickAlmost->reset();
}



void  Tricks::resetFrameInfo()
{
  mColBumperOver.clear();
  mColBumperLeft.clear();
  mColBumperRight.clear();
}

void  Tricks::onColBumperOver( const int aId )
{
  mColBumperOver.push_back(aId);
}

void  Tricks::onColBumperLeft( const int aId )
{
  mColBumperLeft.push_back(aId);
}

void  Tricks::onColBumperRight( const int aId )
{
  mColBumperRight.push_back(aId);
}

void  Tricks::stateMachine( const float  aLapsedTime, 
            const bool    aIsInContact,
            const bool    aHadContact,
            const bool    aUsedNitro,
            const Ball&   aBall )
{

  switch ( mState )
  {
  case ready:
    if ( aIsInContact || aHadContact )
    {            
      mStateStartedJump = false;
      mStateLandedJump  = false;
      mStateJumpFalling = false;
      mStateInLand      = true;
      mState            = inLand;
      //*mLog->mFileEvents << "In land: " << mStateJumpMaxHeight << std::endl;
    }
    break;

  case inLand:
    if ( !(aIsInContact || aHadContact) )
    {
      mStateJumpStartHeight   = aBall.mBallBody->getxWS().y;
      mStateJumpStartPosition = aBall.mBallBody->getxWS();
      mStateJumpMaxHeight     = 0;
      mStateJumpMaxDistance   = 0;
      mStateStartedJump = true;
      mStateLandedJump  = false;
      mStateJumpFalling = false;
      mStateInLand      = false;
      mState = inAir;
      //*mLog->mFileEvents << "jump: " << mStateJumpMaxHeight << std::endl;
    }
    break;

  case inAir:
    if ( aIsInContact || aHadContact )
    {
      mStateStartedJump = false;
      mStateLandedJump  = true;      
      mStateJumpFalling = false;
      mStateInLand      = false;
      mState = inLand;
      //*mLog->mFileEvents << "Landed: " << mStateJumpMaxHeight << std::endl;
    }
    if ( ! (aIsInContact || aHadContact) 
        && aBall.mBallBody->getxWS().y < mPreviousPosition.y )
    {
      mStateStartedJump = false;
      mStateLandedJump  = false;      
      mStateJumpFalling = true;
      mStateInLand      = false;
      mState = inAirFalling;
      //*mLog->mFileEvents << "falling: " << mStateJumpMaxHeight << std::endl;
    }
    break;

  case inAirFalling:
    if ( aIsInContact || aHadContact )
    {
      //*mLog->mFileEvents << "Landed: " << mStateJumpMaxHeight << std::endl;
      mStateStartedJump = false;
      mStateLandedJump  = true;      
      mStateJumpFalling = false;
      mStateInLand      = false;
      mState = inLand;
    }
    break;
  }

  mPreviousPosition = aBall.mBallBody->getxWS();

  if ( mState == inAirFalling || mState == inAir )
  {
    mStateJumpMaxHeight   = DxMath::max( mStateJumpMaxHeight, aBall.mBallBody->getxWS().y - mStateJumpStartHeight );
    mStateJumpMaxDistance = DxMath::max( mStateJumpMaxDistance, DxMath::distance( mStateJumpStartPosition, aBall.mBallBody->getxWS()) );  
    mStateJumpAbsHeight   = fabs( aBall.mBallBody->getxWS().y - mStateJumpStartHeight );
  }
}