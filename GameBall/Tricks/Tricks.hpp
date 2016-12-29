#ifndef _Tricks_HPP_
#define _Tricks_HPP_

#include  "GameBall/Entities/Ball.hpp"
#undef max

class Log;
class TrackPath;
class TrackEdge;

class Trick_AirTurbo;
class Trick_GettingHigh;
class Trick_BigFall;
class Trick_MoodSwing;
class Trick_Ponder;
class Trick_OverBumper;
class Trick_SwingerLife;
class Trick_Lane;
class Trick_Grind;
class Trick_Almost;



class Tricks{
public:
  ~Tricks();
  
  Tricks();
  
  void  reset();
  void  resetFrameInfo();
  void  update( const float   aLapsedTime, 
                const float  aGameTime, 
                const bool    aIsInContact,
                const bool    aHadContact,
                const bool    aUsedNitro,
                const TrackPath*    aCurrentTrackPath,
                const TrackEdge*    aCurrentTrackEdge,
                const float   aDistanceToCenterOfTrack,
                const float   aTrackWidth,
                const Ball&   aBall );

  

  std::string   displayText() const;
  int           punctuation() const;


  void    addPunctuation( const int aPoints, const std::string& aTrickName );
  Log*                mLog;

  //  callbacks for colliders
  void  onColBumperOver ( const int aId );
  void  onColBumperLeft ( const int aId );
  void  onColBumperRight( const int aId );

private:
  float   mGameCurrentTime;
  float   mComboStartTime;
  float   mLapsedTimeCombo;  
  bool    mInCombo;
  int     mComboPoints;
  float   mTotalPoints;

public:
  float   mMaxComboPoints;
  float   mMaxComboStartTime;
  float   mMaxComboEndTime;
private:
  std::vector<std::string>  mTricksInCombo;
  std::string               mMessage;

private:
  Trick_AirTurbo*     mTrickAirTurbo;
  Trick_GettingHigh*  mTrickHigh;
  Trick_BigFall*      mTrickFall;
  Trick_MoodSwing*    mTrickMood;
  Trick_Ponder*       mTrickPonder;
  Trick_OverBumper*   mTrickOverBumper;
  Trick_SwingerLife*  mTrickSwinger;
  Trick_Lane*         mTrickLane;
  Trick_Grind*        mTrickGrind;
  Trick_Almost*       mTrickAlmost;
  
private:  
  const static float mComboMultiplier;
  const static float mMaxTimeCombo;

  

  void    endCombo();
  void    prepareMessage();

private:
  //  info resetted per frame
  std::vector<int>  mColBumperOver;
  std::vector<int>  mColBumperLeft;
  std::vector<int>  mColBumperRight;

  //  state machine
  int           mState;
  float         mStateJumpStartHeight;
  D3DXVECTOR3   mStateJumpStartPosition;
  D3DXVECTOR3   mPreviousPosition;

  float         mStateJumpMaxHeight;
  float         mStateJumpMaxDistance;
  float         mStateJumpAbsHeight;

  bool          mStateLandedJump;
  bool          mStateJumpFalling;
  bool          mStateStartedJump;
  bool          mStateInLand;

  void  stateMachine( const float  aLapsedTime, 
            const bool    aIsInContact,
            const bool    aHadContact,
            const bool    aUsedNitro,
            const Ball&   aBall );

};

#endif  //_Tricks_HPP_
