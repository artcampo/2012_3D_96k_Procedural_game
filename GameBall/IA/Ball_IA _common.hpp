#ifndef _GameBP_Ball_IA_common_HPP_
#define _GameBP_Ball_IA_common_HPP_

#include "dx_misc.hpp"

namespace BallIA{

  class Target{
    public:
      D3DXVECTOR3   point;
      bool          hasEntity;      
      float         entityBitangentComponent;
      D3DXVECTOR3   splineBitangent;
      float         splineT;
  };
};



#endif  //_GameBP_Ball_IA_common_HPP_
