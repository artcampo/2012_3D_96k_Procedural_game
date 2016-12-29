#ifndef			DX_MODEL_BASIC
#define			DX_MODEL_BASIC


#include "dx_misc.hpp"

#include		<vector>
#include		<queue>

class CubeModel {
public:
	D3DXVECTOR3		center;			//in world coordinates
	D3DXMATRIX	  frameBasis;
	float					xsize;			//in its own basis
	float					ysize;
	float					zsize;
	int				    id_side, id_linearorder;

	CubeModel() {};
};




///////////////////////////////////////////////////////////////////////////////
//		DxCubeModel
///////////////////////////////////////////////////////////////////////////////


class DxCubeModel {

public:
  DxCubeModel       (){};
	void ConvertToDx	( CubeModel c );

private:
  std::vector<D3DXVECTOR3>	  _vertices;
	int			              _nTriangles;
  unsigned int          _nVertices;

  friend class DxCubeModelPool;

};


class DxCubeModelPool {

private:
	std::vector<D3DXVECTOR3>	    _vertices;
	int			                _nTriangles;
  unsigned int            _nVertices;
  ID3D10Buffer*           mVertexBuffer;  

public:

  DxCubeModelPool     ( );
  void addCubeModel   ( DxCubeModel* cyl );

	bool createVB   	  ( );
	void Display		    ( );

};

///////////////////////////////////////////////////////////////////////////////
//		DxArchCubeModel
///////////////////////////////////////////////////////////////////////////////


class DxArchCubeModel {

public:
  DxArchCubeModel(
      const D3DXVECTOR3* pa,
      const D3DXVECTOR3* pb,
      const D3DXMATRIX*  ma,
      const D3DXMATRIX*  mb,
      float              yLength,
      float              zLength,
      int                side
      );

private:
	std::vector<VertexPosNor>	  _vertices;
	int			              _nTriangles;
  unsigned int          _nVertices;

  int                   _pos; //temp var used to create buffer
  void makeQuad ( const D3DXVECTOR3* p0, const D3DXVECTOR3* p1,
                            const D3DXVECTOR3* p2, const D3DXVECTOR3* p3);

  friend class DxArchCubeModelPool;

};


class DxArchCubeModelPool {

private:
	std::vector<VertexPosNor>	    _vertices;
	int			                _nTriangles;
  unsigned int            _nVertices;
  
  ID3D10Buffer*           mVertexBuffer;

public:

  DxArchCubeModelPool   ( );
  void addArchCubeModel ( DxArchCubeModel* cyl );

	bool createVB   	  ( );
	void Display		    ( );

};


#endif