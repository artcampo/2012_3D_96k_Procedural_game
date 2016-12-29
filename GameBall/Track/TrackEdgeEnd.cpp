#include  "GameBall/Track/TrackEdgeEnd.hpp"

TrackEdgeEnd::~TrackEdgeEnd()
{
  delete mSplineSlice;
}

TrackEdgeEnd::TrackEdgeEnd ( const D3DXVECTOR3& mPoints, 
              const std::vector<D3DXVECTOR3>& mBezierPointsSlice,
              const D3DXVECTOR3& aTangent,
              const D3DXVECTOR3& aNormal )              
{  
  mSplineSlice  = new SplineBezierTMapped( &mBezierPointsSlice[0], mBezierPointsSlice.size(), 15 );
}

void  TrackEdgeEnd::generateLoftSurface ()
{  

}

DxLodModel<VertPNTUV>* TrackEdgeEnd::model() const
{
  DxLodModel<VertPNTUV>*   mesh;

  return mesh;
}

MeshCol* TrackEdgeEnd::meshCol() const
{  
  MeshCol* m;
  return m;
}


std::vector<MeshCol*> TrackEdgeEnd::meshColTri() const
{  
  std::vector<MeshCol*>     m  ( 69 );

  return m;
}

