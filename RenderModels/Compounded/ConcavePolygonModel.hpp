#ifndef			_CONCAVE_POLYGON_MODEL_HPP
#define			_CONCAVE_POLYGON_MODEL_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;
#include		"RenderModels\Basics/TriangleModel.hpp"

class ConcavePolygonModel {
public:


  ConcavePolygonModel   ( std::vector<D3DXVECTOR3>& aVertices );

  void  storeMesh ( std::vector<VertexPosNor>& aMesh );
  
  void          scale( float aScale);
  D3DXVECTOR3   getIthPoint ( int i );
  D3DXVECTOR3   getNormal();
  int           getNumVertices();
  bool          isClockwise();
  void          forceClockwise();

private:
  std::vector<D3DXVECTOR3>	mVertices;
  int                       mNumVertices;
  D3DXVECTOR3   center();
  

};



#endif
