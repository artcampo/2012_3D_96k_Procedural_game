#ifndef _SingleBoxPP_H
#define _SingleBoxPP_H

#include "dx_misc.hpp"
#include "Global.hpp"


class SingleBoxP
{
public:

	SingleBoxP( float aScale );
	~SingleBoxP();

	void init();
  void display();

  ID3D10Buffer* getVertexBuffer();
  ID3D10Buffer* getIndexBuffer();
  DWORD getNumFaces();

private:
	DWORD mNumVertices;
	DWORD mNumFaces;
  float mScale;
	
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;  
};

#endif // BOX_H
