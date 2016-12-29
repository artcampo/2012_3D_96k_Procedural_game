#ifndef _TRACK_HISTOGRAM_HPP_
#define _TRACK_HISTOGRAM_HPP_

#include  <vector>



class TrackHistogram {
public:  
  //  construction
  TrackHistogram()
    : mNumEdges(0)
  {
    mInVerticesDegree.resize(4);
    mOutVerticesDegree.resize(4);
    for ( int i = 0; i < 3 ; ++i )
    {
      mInVerticesDegree[i] = 0;
      mOutVerticesDegree[i] = 0;
    }
  };

  void inVertexDegree ( const int aDeg )
  {
    if ( aDeg > 3 )
      ++mInVerticesDegree[3];
    else
      ++mInVerticesDegree[aDeg];
  };

  void outVertexDegree ( const int aDeg )
  {
    if ( aDeg > 3 )
      ++mOutVerticesDegree[3];
    else
      ++mOutVerticesDegree[aDeg];
  };

  std::vector<int>    mInVerticesDegree;
  std::vector<int>    mOutVerticesDegree;
  int                 mNumEdges;
  

};


#endif  //_TRACK_HISTOGRAM_HPP_
