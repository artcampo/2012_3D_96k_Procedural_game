#ifndef _MeanMax_HPP_
#define _MeanMax_HPP_


template <class T>
class MeanMax{

private:
  
  int   mCount;
  T     mSum;
  T     mMin;
  T     mMax;
  
/*
  int   mCount;
  int     mSum;
  int     mMin;
  int     mMax;*/

public:
  MeanMax ():
      mCount(0),
        mSum(0)
  {    
  };

  void count ( const T& aVal )
  {
    if ( mCount == 0 )
    {
      mMin    = aVal;
      mMax    = aVal;
      mSum    = aVal;
      mCount  = 1;
    }
    else
    {
      ++mCount;
      mSum    += aVal;
      if ( aVal < mMin ) mMin = aVal;
      if ( aVal > mMax ) mMax = aVal;
    }
  };

  T mean() const
  {
    return (mSum/mCount);
  };

  T minimum() const
  {
    return mMin;
  };

  T maximum() const
  {
    return mMax;
  };
  
};

#include "CountMeanMax.cpp"

#endif  //  _STL_HELPERS_HPP_
