#include "SingleBoxP.hpp"

SingleBoxP::SingleBoxP( float aScale ): 
  mNumVertices(8), 
  mNumFaces(12), //2 per quad 
  mVB(0), 
  mIB(0),
  mScale( aScale )
{  
}
 
SingleBoxP::~SingleBoxP()
{  	
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);	
}

void SingleBoxP::init()
{
  float x, y, z, scale;

  x     = 0.0f;
  y     = 0.0f;
  z     = 0.0f;
  scale = mScale;

	// Create vertex buffer
  VertexPos vertices[] =
  {
    {D3DXVECTOR3(x-1.0f, y-1.0f, z-1.0f)},
	  {D3DXVECTOR3(x-1.0f, y+1.0f, z-1.0f)},
	  {D3DXVECTOR3(x+1.0f, y+1.0f, z-1.0f)},
	  {D3DXVECTOR3(x+1.0f, y-1.0f, z-1.0f)},
	  {D3DXVECTOR3(x-1.0f, y-1.0f, z+1.0f)},
	  {D3DXVECTOR3(x-1.0f, y+1.0f, z+1.0f)},
	  {D3DXVECTOR3(x+1.0f, y+1.0f, z+1.0f)},
	  {D3DXVECTOR3(x+1.0f, y-1.0f, z+1.0f)},  
  };

	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


  D3D10_BUFFER_DESC vbd;
  vbd.Usage = D3D10_USAGE_IMMUTABLE;
  vbd.ByteWidth = sizeof(VertexPos) * mNumVertices;
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

ID3D10Buffer* SingleBoxP::getIndexBuffer()
{
  return mIB;
}

ID3D10Buffer* SingleBoxP::getVertexBuffer()
{
  return mVB;
}

DWORD SingleBoxP::getNumFaces()
{
  return mNumFaces;
}

void SingleBoxP::display()
{
  ID3D10Buffer* mVB;
  ID3D10Buffer* mIB;

  mIB = getIndexBuffer();
  UINT stride = sizeof(VertexPos);
  UINT offset = 0;
  Global::md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

  mVB = getVertexBuffer();
  Global::md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);      
  Global::md3dDevice->DrawIndexed(getNumFaces()*3, 0, 0);     	
}
