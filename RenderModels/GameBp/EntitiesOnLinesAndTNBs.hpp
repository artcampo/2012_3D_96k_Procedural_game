#ifndef			_ENTITIES_TNB_HPP
#define			_ENTITIES_TNB_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;
#include		"RenderModels/Basics/TriangleModel.hpp"
#include    "RenderModels/Compounded/AxisModel.hpp"
#include    "RenderModels/Basics/LineListModel.h"

#include  "GameBall/Entities/Bumper.hpp"
#include  "GameBall/Entities/Arch.hpp"

class EntitiesOnLinesAndTNBs {
public:

  EntitiesOnLinesAndTNBs   ( std::vector<Arch*>& aArchs, std::vector<Bumper*>& aBumpers );
  
  void display  ();

private:  
  std::vector<AxisModelPC*>     mAxis;

};



#endif
