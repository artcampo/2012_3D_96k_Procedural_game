#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_OverBumper.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;



Trick_OverBumper::Trick_OverBumper( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0)
{  
}

void  Trick_OverBumper::update( const bool    aHasPassedOver)
{
  if( aHasPassedOver )
  {
    mTrickManager->addPunctuation(  Score_OverBumper::mPoints +
      Score_OverBumper::mPoints/10*mNum,
                                    std::string("Over bumper") );                                      
    ++mNum;  
  }
}

void  Trick_OverBumper::reset()
{
  mNum = 0;

}