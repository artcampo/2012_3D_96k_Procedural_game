#include		"RenderModels/GameBp/EntitiesOnLinesAndTNBs.hpp"

EntitiesOnLinesAndTNBs::EntitiesOnLinesAndTNBs( std::vector<Arch*>& aArchs, std::vector<Bumper*>& aBumpers )
{
  for ( int i = 0; i < aArchs.size() ; ++i )
  {
    std::vector<D3DXVECTOR3> points;

    D3DXVECTOR3 c = aArchs[i]->mCenter;
    D3DXVECTOR3 _i = D3DXVECTOR3(1,0,0);
    D3DXVECTOR3 j = D3DXVECTOR3(0,1,0);
    D3DXVECTOR3 k = D3DXVECTOR3(0,0,1);
    D3DXMATRIX b = *aArchs[i]->basis();
    //D3DXMatrixInverse( &b, NULL, &b );
    D3DXVec3TransformCoord ( &_i, &_i, &b );
    D3DXVec3TransformCoord ( &j, &j, &b );
    D3DXVec3TransformCoord ( &k, &k, &b );
    c += j;

    float l = 6.0f;
    points.push_back( c );
    points.push_back( c + l*_i );
    points.push_back( c );
    points.push_back( c + l*j );
    points.push_back( c );
    points.push_back( c + l*k );

    mAxis.push_back( new AxisModelPC( points ) );
  }

  for ( int i = 0; i < aBumpers.size() ; ++i )
  {
    std::vector<D3DXVECTOR3> points;

    D3DXVECTOR3 c = aBumpers[i]->mCenter;
    D3DXVECTOR3 _i = D3DXVECTOR3(1,0,0);
    D3DXVECTOR3 j = D3DXVECTOR3(0,1,0);
    D3DXVECTOR3 k = D3DXVECTOR3(0,0,1);
    D3DXMATRIX b = aBumpers[i]->basis();
    //D3DXMatrixInverse( &b, NULL, &b );
    D3DXVec3TransformCoord ( &_i, &_i, &b );
    D3DXVec3TransformCoord ( &j, &j, &b );
    D3DXVec3TransformCoord ( &k, &k, &b );
    c += j;

    float l = 6.0f;
    points.push_back( c );
    points.push_back( c + l*_i );
    points.push_back( c );
    points.push_back( c + l*j );
    points.push_back( c );
    points.push_back( c + l*k );

    mAxis.push_back( new AxisModelPC( points ) );
  }
}

void EntitiesOnLinesAndTNBs::display()
{
  for ( int i = 0; i < mAxis.size() ; ++i )
    mAxis[i]->display();
}

