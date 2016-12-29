#ifndef _TrackTimer_HPP_
#define _TrackTimer_HPP_

#include <string>
#include <sstream>
#include <iostream>

class TrackTimer{
public:
  ~TrackTimer(){};
  static TrackTimer*  timerUp   ( const float aMaxTime );
  static TrackTimer*  timerDown ( const float aCurrentTime );

  std::string displayString() const;
  bool        isTimeFinished() const;

  void  updateTime( const float aLapsedTime );
  void  addTime( const float aTime );

  void  stop();
  void  resume();
  
private:
  TrackTimer(){};
private:
  bool    mIsUp;
  float   mCurrentTime;
  float   mMaxTime;
  bool    mStopped;


};

#endif  //_TrackTimer_HPP_
