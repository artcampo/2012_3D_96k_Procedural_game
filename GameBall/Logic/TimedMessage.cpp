#include  "GameBall/Logic/TimedMessage.hpp"
#include  "GameBall/Logic/TimeLimitedEvent.hpp"

TimedMessage::TimedMessage( const float aTimeSeconds, const std::string& aMessage ) :
  mTimeLimit(  new TimeLimitedEvent(aTimeSeconds*100, true) ),
  mMessage( aMessage )
{
}


TimedMessage::~TimedMessage()
{ delete mTimeLimit; }