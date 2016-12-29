#ifndef _SphereModel_PN_HPP
#define _SphereModel_PN_HPP

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels\Basics\linelistmodel.h"

using namespace std;

class SphereModel_PN
{
public:
    ~SphereModel_PN();
    SphereModel_PN( int numberVerticesPerEdge );
    void display();

    D3DXCOLOR&  getColour();
private:

    DxMeshModelBatch<VertexPosNor>* mModel;

private:
    std::vector<VertexPosNor> sphere(int size);

};

#endif // _SphereModel_PN_H
