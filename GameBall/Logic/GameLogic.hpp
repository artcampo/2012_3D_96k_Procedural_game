#ifndef _GameLogic_HPP_
#define _GameLogic_HPP_

namespace GameLogic{
  class CheckPoint{
  public:
    CheckPoint(){};
    D3DXVECTOR3           pos;
    Camera3rdPersonView   camera3rd;
  };
};

#endif  //_GameLogic_HPP_
