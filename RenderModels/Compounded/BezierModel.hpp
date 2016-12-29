#ifndef BEZIERMODEL_HPP
#define BEZIERMODEL_HPP

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels\Basics\linelistmodel.h"


class BezierModel
{
public:
    ~BezierModel();
    BezierModel( std::vector<D3DXVECTOR3>& aPointsCurve,
                 D3DXCOLOR& aColour);
    void displayLines();

    D3DXCOLOR&  getColour();
private:
    D3DXCOLOR mColour;
    LineListModel<VertexPos>*    mLines;

};

#endif // BEZIERMODEL_H
