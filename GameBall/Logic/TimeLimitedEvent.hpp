#ifndef _TimeLimitedEvent_HPP_
#define _TimeLimitedEvent_HPP_


class TimeLimitedEvent {
public:  
  TimeLimitedEvent() 
  {
    mInUse      = false;
  }
  TimeLimitedEvent( const float aTime, const bool aInUse )
  {
    mMaxTime    = aTime;
    mLapsedTime = 0.0f;
    mInUse      = aInUse;
  }

  void  update( const float aLapsedTime )
  {
    mLapsedTime += aLapsedTime;
  }  

  bool  acting() const
  {
    return (mInUse & ( mLapsedTime < mMaxTime ));
  }

  
public:
  float mMaxTime;
  float mLapsedTime;
  bool  mInUse;

};


#endif  //TimeLimitedEvent_HPP_
