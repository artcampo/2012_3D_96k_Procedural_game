#ifndef _TimedMessage_HPP_
#define _TimedMessage_HPP_

#include <string>
#include <sstream>
#include <iostream>

class TimeLimitedEvent;
//#include  "GameBall/Logic/TimeLimitedEvent.hpp"

class TimedMessage{
public:
  ~TimedMessage();
  TimedMessage( const float aTimeSeconds, const std::string& aMessage );
  TimedMessage(){};
  

public:  
  TimeLimitedEvent*     mTimeLimit;
  std::string           mMessage;


};

#endif  //_TimedMessage_HPP_
