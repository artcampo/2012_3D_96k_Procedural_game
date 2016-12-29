#ifndef			_TRIANGLE_MODEL_HPP
#define			_TRIANGLE_MODEL_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;



class TriangleModel {
public:

  void  storeMesh ( std::vector<VertexPosNor>& aMesh );
  
  TriangleModel( std::vector<D3DXVECTOR3>& aVertices );
  TriangleModel( D3DXVECTOR3& aV1, D3DXVECTOR3& aV2, D3DXVECTOR3& aV3 );


private:
  std::vector<D3DXVECTOR3>	mVertices;
  int			mNumVertices;



};



#endif
