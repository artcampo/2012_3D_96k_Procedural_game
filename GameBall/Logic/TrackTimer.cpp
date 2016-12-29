#include  "GameBall/Logic/TrackTimer.hpp"

TrackTimer*  TrackTimer::timerUp   ( const float aMaxTime )
{
  TrackTimer* t   = new TrackTimer();
  t->mIsUp        = true;
  t->mCurrentTime = 0;
  t->mMaxTime     = aMaxTime;
  t->mStopped     = false;
  return t;
}

TrackTimer*  TrackTimer::timerDown ( const float aCurrentTime )
{
  TrackTimer* t = new TrackTimer();
  t->mIsUp = false;
  t->mCurrentTime = aCurrentTime;  
  t->mStopped     = false;  
  return t;
}


std::string TrackTimer::displayString() const
{
  std::string s;
  if ( mIsUp )
  {
    std::stringstream st;
    st.setf( std::ios_base::fixed, std::ios_base::floatfield );
    st.precision(1);
    st << mCurrentTime/100.0f <<" / " << mMaxTime/100.0f;
    s = st.str();
  }
  else
  {    
    std::stringstream st;
    st.setf( std::ios_base::fixed, std::ios_base::floatfield );
    st.precision(1);
    st << mCurrentTime/100.0f ;
    s = st.str();
  }

  return s;
}

bool        TrackTimer::isTimeFinished() const
{
  bool ret;

  if ( mIsUp )
    ret = (mCurrentTime < mMaxTime);
  else
    ret = (mCurrentTime <= 0.0f);

  return ret;
}


void  TrackTimer::updateTime( const float aLapsedTime )
{
  if ( !mStopped )
  {
    ( mIsUp ) ? mCurrentTime += aLapsedTime : mCurrentTime -= aLapsedTime;
  }
}

void  TrackTimer::addTime( const float aTime )
{
  if ( !mStopped )
    if ( !mIsUp )
      mCurrentTime += aTime;
}


void  TrackTimer::stop()
{
  mStopped = true;
}

void  TrackTimer::resume()
{
  mStopped = false;
}
