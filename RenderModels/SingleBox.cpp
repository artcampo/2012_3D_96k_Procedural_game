//=======================================================================================
// SingleBox.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "SingleBox.hpp"

SingleBox::SingleBox(): 
  mNumVertices(8), 
  mNumFaces(12), //2 per quad 
  mVB(0), 
  mIB(0)
{  
}
 
SingleBox::~SingleBox()
{  	
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);	
}

void SingleBox::init()
{
  float x, y, z, scale;

  x     = 0.0f;
  y     = 0.0f;
  z     = 0.0f;
  scale = 10.0f;

	// Create vertex buffer
  VertPNT vertices[] =
  {
    {D3DXVECTOR3(x-1.0f, y-1.0f, z-1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x-1.0f, y+1.0f, z-1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x+1.0f, y+1.0f, z-1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x+1.0f, y-1.0f, z-1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x-1.0f, y-1.0f, z+1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x-1.0f, y+1.0f, z+1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x+1.0f, y+1.0f, z+1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
	  {D3DXVECTOR3(x+1.0f, y-1.0f, z+1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },  
  };

	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


  D3D10_BUFFER_DESC vbd;
  vbd.Usage = D3D10_USAGE_IMMUTABLE;
  vbd.ByteWidth = sizeof(VertPNT) * mNumVertices;
  vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = vertices;
  HR(Global::md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};

	D3D10_BUFFER_DESC ibd;
  ibd.Usage = D3D10_USAGE_IMMUTABLE;
  ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
  ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = indices;
  HR(Global::md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

ID3D10Buffer* SingleBox::getIndexBuffer() const
{
  return mIB;
}

ID3D10Buffer* SingleBox::getVertexBuffer() const
{
  return mVB;
}

DWORD SingleBox::getNumFaces() const
{
  return mNumFaces;
}

void SingleBox::display()  const
{
  ID3D10Buffer* mVB;
  ID3D10Buffer* mIB;

  mIB = getIndexBuffer();
  UINT stride = sizeof(VertPNT);
  UINT offset = 0;
  Global::md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

  mVB = getVertexBuffer();
  Global::md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);      
  Global::md3dDevice->DrawIndexed(getNumFaces()*3, 0, 0);     	
}