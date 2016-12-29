#ifndef			_TRACK_ON_LINES_HPP
#define			_TRACK_ON_LINES_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;
#include		"RenderModels/Basics/TriangleModel.hpp"
#include    "RenderModels/Compounded/AxisModel.hpp"
#include    "RenderModels/Basics/LineListModel.h"

class TrackLinesAndTNBs {
public:


  TrackLinesAndTNBs   (){};

  void setEdges ( std::vector<VertexPC>& mVertices );
  void setBasis ( std::vector< std::vector<D3DXVECTOR3> >& aBasis );
  void display  ();

private:
  LineListModel<VertexPC>*    mEdges;
  std::vector<AxisModelPC*>     mAxis;

};



#endif
