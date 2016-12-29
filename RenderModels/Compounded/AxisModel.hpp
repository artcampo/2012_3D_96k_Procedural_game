#ifndef _AXISMODEL_H
#define _AXISMODEL_H

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels/DxMeshModel.hpp"

class AxisModel
{
public:
    // Pass center, followed from the 3-axis (thus 6 vertices, line list)
    AxisModel( std::vector<D3DXVECTOR3>& mVertices );
    ~AxisModel();
    void display();
    void displayX();
    void displayY();
    void displayZ();
    D3DXCOLOR  getColourX();
    D3DXCOLOR  getColourY();
    D3DXCOLOR  getColourZ();
private:
    DxMeshModelBatch<VertexPosNorCol>* mMeshX;
    DxMeshModelBatch<VertexPosNorCol>* mMeshY;
    DxMeshModelBatch<VertexPosNorCol>* mMeshZ;
};

class AxisModelPC
{
public:
    // Pass center, followed from the 3-axis (thus 6 vertices, line list)
    AxisModelPC( std::vector<D3DXVECTOR3>& mVertices );
    ~AxisModelPC();
    void display();
    void displayX();
    void displayY();
    void displayZ();

private:
    DxMeshModelBatch<VertexPC>* mMeshX;
    DxMeshModelBatch<VertexPC>* mMeshY;
    DxMeshModelBatch<VertexPC>* mMeshZ;
};

#endif
