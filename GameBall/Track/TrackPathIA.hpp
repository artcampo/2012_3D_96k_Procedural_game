#ifndef _GameTrackPath_IA_HPP_
#define _GameTrackPath_IA_HPP_

#include "dx_misc.hpp"
#include "Geometry/DxMathHelpers.hpp"

class TrackPath;
class TrackEdge;

class EdgePath{
public:
  D3DXVECTOR3 beging;
  D3DXVECTOR3 center;  
  D3DXVECTOR3 ending;
  TrackPath*  tPath;
  TrackEdge*  tEdge;

  float minDistance( const D3DXVECTOR3& aPoint ) 
  {    
    float d1 = DxMath::distance( aPoint, beging );
    float d2 = DxMath::distance( aPoint, center );
    float d3 = DxMath::distance( aPoint, ending );
    return std::min<float>(d1, std::min<float>(d2, d3) );     
  };

};


#endif  //_GameTrackPath_IA_HPP_
