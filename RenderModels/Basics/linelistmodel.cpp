#include "RenderModels\Basics\linelistmodel.h"

template <class VertexLayout>
LineListModel<VertexLayout>::LineListModel( std::vector<VertexLayout>& mVertices, D3DXCOLOR& aColour )
{
  DxMeshModel<VertexLayout>* mesh;
  mesh  = new DxMeshModel<VertexLayout>( sizeof(VertexLayout), mVertices.size(), &mVertices[0] );
  mMesh = new DxMeshModelBatch<VertexLayout>( sizeof(VertexLayout), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
  mMesh->addMeshModel( mesh );
  mMesh->createVB( );
}

template <class VertexLayout>
LineListModel<VertexLayout>::~LineListModel()
{
  delete mMesh;
}

template <class VertexLayout>
void LineListModel<VertexLayout>::display()
{  
  mMesh->DisplaySansShader();
}

/*
  Used for batching later
*/
template <class VertexLayout>
DxMeshModel<VertexLayout> 
static LineListModel<VertexLayout>::getMesh
  ( const std::vector<VertexLayout>& mVertices, const D3DXCOLOR& aColour )
{
  DxMeshModel<VertexLayout> m(sizeof(VertexLayout), mVertices.size(), &mVertices[0] );
  return m;
}