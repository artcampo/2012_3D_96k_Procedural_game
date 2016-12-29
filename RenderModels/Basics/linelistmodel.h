#ifndef LINELISTMODEL_H
#define LINELISTMODEL_H

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels/DxMeshModel.hpp"

template <class VertexLayout>
class LineListModel
{
public:
    DxMeshModel<VertexLayout> static getMesh( const std::vector<VertexLayout>& mVertices, const D3DXCOLOR& aColour );
public:
    LineListModel( std::vector<VertexLayout>& mVertices, D3DXCOLOR& aColour );
    ~LineListModel();
    void display();
private:
    D3DXCOLOR mColour;
    DxMeshModelBatch<VertexLayout>* mMesh;
};

#include "linelistmodel.cpp"

#endif // LINELISTMODEL_H
