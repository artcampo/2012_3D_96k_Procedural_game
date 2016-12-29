#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_SwingerLife.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;


enum states{
  ready,  
  alreadyPassedAtLeastOne
};

Trick_SwingerLife::Trick_SwingerLife( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(ready)
{  
}

/*
Trick_SwingerLife::~Trick_SwingerLife()
{ }
*/
  
void  Trick_SwingerLife::update( 
                const std::vector<int>& aColBumperLeft, 
                const std::vector<int>& aColBumperRight)
{
  switch ( mState )
  {

  case ready:
    if( !aColBumperLeft.empty() || !aColBumperRight.empty() )
    {
      if ( !aColBumperLeft.empty() )
      {
        mPrevId = aColBumperLeft[0];
        mPassedAtLeft = true;
      }
      else
      {
        mPrevId = aColBumperRight[0];
        mPassedAtLeft = false;
      }
      
      //*mTrickManager->mLog->mFileEvents << "Swinger_start: ";
      if ( mPassedAtLeft )  *mTrickManager->mLog->mFileEvents << " at left" << std::endl;
      else                  *mTrickManager->mLog->mFileEvents << " at right" << std::endl;

      mState        = alreadyPassedAtLeastOne;
    }
    break;

  case alreadyPassedAtLeastOne:

    if( aColBumperLeft.empty() && aColBumperRight.empty() )
      return;

    /* 
    *mTrickManager->mLog->mFileEvents << "Swinger_next: ";
    if ( !aColBumperLeft.empty() )
      *mTrickManager->mLog->mFileEvents << " at left" << std::endl;
    if ( !aColBumperRight.empty() )
      *mTrickManager->mLog->mFileEvents << " at right" << std::endl;
    */

    if ( !aColBumperLeft.empty() && mPassedAtLeft )
      mState = ready;
    else
    {
      if ( !aColBumperRight.empty() && !mPassedAtLeft )
        mState = ready;
      else
      {
        if ( !aColBumperRight.empty() && aColBumperRight[0] != mPrevId )
        {
          mPrevId = aColBumperRight[0];
          mPassedAtLeft = false;
          apply();
          return;
        }
        else
          if( !aColBumperLeft.empty() && aColBumperLeft[0] != mPrevId )
          {
            mPrevId = aColBumperLeft[0];
            mPassedAtLeft = true;
            apply();
            return;
          }

       mState = ready;

      }
    }

    break;
  }
  
}

void  Trick_SwingerLife::apply()
{
  mTrickManager->addPunctuation(  Score_SwingerLife::mPoints +
    Score_SwingerLife::mPoints/10*mNum,
                                  std::string("Swinger") );                                      
  ++mNum;
}

void  Trick_SwingerLife::reset()
{
  mNum = 0;
  mState = ready;
}