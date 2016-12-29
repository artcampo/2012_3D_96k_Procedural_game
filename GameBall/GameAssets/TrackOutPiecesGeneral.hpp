#ifndef _TrackOutPiecesGeneral_HPP_
#define _TrackOutPiecesGeneral_HPP_

#include  "GameBall/GameAssets/TrackOutPiecesGeneral.hpp"

namespace StoredPiecesBalancers{

  /////////////////////////////////////////////////////////////////////////
  //  Data describing the various balancers
  /////////////////////////////////////////////////////////////////////////

  namespace General{
    const static int subsMinDistance    = 200;  
    const static int subsMinAzimuth     = 75.0f;   //in degrees
    const static int subsMaxAzimuth     = 115.0f;   //in degrees
    const static int subsMaxPolar       = 40;   //in degrees
    const static int tanAbsoberDistance = 50;
    
  };

};

#endif  //_TrackOutPiecesBalancers_HPP_
