#ifndef			_RECTANGLE_MODEL_HPP
#define			_RECTANGLE_MODEL_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;



class RectangleModel {
public:


  RectangleModel ( const D3DXVECTOR3& aCenter, const float aLengthU, const float aLengthV );
  ~RectangleModel ()
  {
    delete mMesh;
  }

  void display()
  {
    mMesh->DisplaySansShader();
  }

private:
  std::vector<D3DXVECTOR3>	mVertices;
  int                       mNumVertices;
  DxMeshModelBatch<VertexPos>*   mMesh;
  

};



#endif
