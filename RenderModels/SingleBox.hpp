#ifndef _SINGLEBOX_H
#define _SINGLEBOX_H

#include "dx_misc.hpp"
#include "Global.hpp"


class SingleBox
{
public:

	SingleBox();
	~SingleBox();

	void init();
  void display() const;

  ID3D10Buffer* getVertexBuffer() const;
  ID3D10Buffer* getIndexBuffer() const;
  DWORD getNumFaces() const;

private:
	DWORD mNumVertices;
	DWORD mNumFaces;
	
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;  
};

#endif // BOX_H
