#include		"RenderModels/GameBp/TrackLinesAndTNBs.hpp"


void TrackLinesAndTNBs::setEdges ( std::vector<VertexPC>& mVertices )
{
  mEdges = new LineListModel<VertexPC>( mVertices, D3DXCOLOR(1,0,0,1 ) );
}

void TrackLinesAndTNBs::display()
{
  mEdges->display();
  for ( int i = 0; i < mAxis.size() ; ++i )
    mAxis[i]->display();
}

void TrackLinesAndTNBs::setBasis( std::vector< std::vector<D3DXVECTOR3> >& aBasis )
{
  mAxis.resize( aBasis.size() );
  for ( int i = 0; i < aBasis.size() ; ++i )
    mAxis[i] = new AxisModelPC( aBasis[i] );
}