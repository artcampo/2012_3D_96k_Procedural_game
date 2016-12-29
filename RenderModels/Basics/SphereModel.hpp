#ifndef SPHEREMODEL_HPP
#define SPHEREMODEL_HPP

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels\Basics\linelistmodel.h"

using namespace std;

class SphereModel
{
public:
    ~SphereModel();
    SphereModel( int numberVerticesPerEdge );
    void display();

    D3DXCOLOR&  getColour();
private:

    DxMeshModelBatch<VertexPos>* mModel;

private:
    std::vector<VertexPos> sphere(int size);

};

#endif // SphereModel_H
