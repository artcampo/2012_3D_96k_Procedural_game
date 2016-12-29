#ifndef			_CYLINDER_MODEL_HPP
#define			_CYLINDER_MODEL_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;



class CylinderModel {
public:

  void  storeMesh ( DxMeshModel<VertexPosNor>** aMesh );
  
  CylinderModel   (){};

  bool init         ( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2 );
  bool init         ( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, D3DXVECTOR3 pd, D3DXVECTOR3 nd );
  bool initDirNext  ( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, D3DXVECTOR3 nd );
  bool initDirPrev  ( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, D3DXVECTOR3 pd );

private:
  std::vector<VertexPosNor>	mVertices;
  int			mNumVertices;

	int			mLengthTesellation;
	int			mRadiusTesellation;

  /*
  D3DXVECTOR3			mP1WS;			//in world coordinates
	D3DXVECTOR3			mP2WS;
	float					  mR1OS;      //in object space
	float					  mR2OS;
  */

  void		BuildModel ( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, bool pdUsed, bool ndUsed, D3DXVECTOR3 pd, D3DXVECTOR3 nd );
  

  D3DXMATRIX  TbnMatrix ( const D3DXVECTOR3& aVect );

};



#endif
