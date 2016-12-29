#include "beziermodel.hpp"

BezierModel::BezierModel( std::vector<D3DXVECTOR3>& aPointsCurve,
                          D3DXCOLOR& aColour)
{
  std::vector<VertexPos> points (aPointsCurve.size());
  for ( int i = 0 ; i < aPointsCurve.size() ; ++i)
    points[i].pos = aPointsCurve[i];

  mColour = aColour;
  mLines  = new LineListModel<VertexPos>( points, aColour );

}

BezierModel::~BezierModel()
{
  delete mLines;
}

void BezierModel::displayLines()
{
  mLines->display();
}


D3DXCOLOR& BezierModel::getColour()
{
  return mColour;
}
