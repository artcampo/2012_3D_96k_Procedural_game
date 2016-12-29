#include  "GameBall/Track.hpp"
#include  "GameBall/Game.hpp"
#include  "Geometry/2dDataStructures/KddTree2d.hpp"

//#include  "Geometry/2dDataStructures/Kddtree2d_EdgePath.cpp"

void Track::generateIaStructures()
{
  std::vector<EdgePath*> edges;

  for ( int i = 0; i < mPath.size(); ++i )
  {
    for ( int j = 0; j < mPath[i]->mEdges.size(); ++j )
    {
      
      EdgePath* e = new EdgePath();
      e->beging = mPath[i]->mEdges[j]->mSplinePath->getPoint( 0.1f );
      e->center = mPath[i]->mEdges[j]->mSplinePath->getPoint( 0.5f );
      e->ending = mPath[i]->mEdges[j]->mSplinePath->getPoint( 0.9f );
      e->tPath  = mPath[i];
      e->tEdge  = mPath[i]->mEdges[j];

      edges.push_back( e );
    }

  }
  mEdgePathsKdd = new Kdd2d::KddTree2d<EdgePath>( edges );  
}


Kdd2d::KddTree2d<EdgePath>* Track::edgePathsKdd() const
{
  return mEdgePathsKdd;
}