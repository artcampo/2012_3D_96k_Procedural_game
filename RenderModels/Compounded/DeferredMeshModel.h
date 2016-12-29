#ifndef _DeferredMeshModel_H
#define _DeferredMeshModel_H

#include <vector>
#include "dx_misc.hpp"

#include "RenderModels\Basics\pointlistmodel.h"
#include "RenderModels\Basics\linelistmodel.h"
#include "RenderModels\DxMeshModel.hpp"

class DeferredMeshModel
{
public:
    ~DeferredMeshModel();
    DeferredMeshModel( std::vector<VertPNT>& aTrianglePoints,
                 D3DXCOLOR& aColour,
                 ID3D10Device* aMd3dDevice);

    void displayTriangles();

private:
  DxMeshModelBatch<VertPNT>* mMeshTriangles;
};

#endif // _DeferredMeshModel_H
