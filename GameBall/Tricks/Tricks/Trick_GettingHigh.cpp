#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_GettingHigh.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;

const float Trick_GettingHigh::mHeight       = 10.0f;
const float Trick_GettingHigh::mHeightExtra  = 30.0f;

enum states{
  ready,
  jump
};

Trick_GettingHigh::Trick_GettingHigh( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(ready)
{  
}

/*
Trick_GettingHigh::~Trick_GettingHigh()
{ }
*/
  
void  Trick_GettingHigh::update(  const double        aLapsedTime, 
                                  const bool          aHasStartedJump,
                                  const bool          aHasLanded,
                                  const bool          aIsStartingJumpFall,
                                  const float         aJumpMaxHeight)
{
  switch (mState)
  {

  case ready:
    if ( aHasStartedJump )
    {
      //*mTrickManager->mLog->mFileEvents << "high: " << aJumpMaxHeight << std::endl;
      mState = jump;
    }
    break;

  case jump:
    if( aHasLanded || aIsStartingJumpFall )
      {
        //*mTrickManager->mLog->mFileEvents << "highEnd: " << aJumpMaxHeight << std::endl;
        if ( aJumpMaxHeight > mHeight)
          {
            if ( aJumpMaxHeight > mHeight && aJumpMaxHeight < mHeightExtra )
              mTrickManager->addPunctuation(  Score_GettingHigh::mPoints +
              Score_GettingHigh::mPoints/10*mNum,
                                              std::string("Getting high") );                                    
            if ( aJumpMaxHeight > mHeightExtra )
              mTrickManager->addPunctuation(  Score_GettingHigh::mPoints +
              Score_GettingHigh::mPoints/10*mNum,
                                              std::string("Getting EXTRA high") );  
            ++mNum;
          }
        mState = ready;
      }
    break;
  }
  
}

void  Trick_GettingHigh::reset()
{
  mState = ready;
  mNum = 0;
}