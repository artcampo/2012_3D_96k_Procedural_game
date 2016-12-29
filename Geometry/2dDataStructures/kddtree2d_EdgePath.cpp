#include "Geometry/2dDataStructures/KddTree2d.hpp"
#include "Geometry/2dDataStructures/KddTree2dSpecializations.hpp"



template <>
void
Kdd2d::KddTree2d<EdgePath>::neighboursRec( typename Kdd2d::KddTree2d<EdgePath>::KddTree2dNode* aNode,
                    bool aOrder, 
                    const float aX, 
                    const float aY,
                    const float aEpsilon,
                    std::vector<EdgePath*>& aPoints )
{
  bool nearLine = false;
  bool isInLeftBranch;

  EdgePath* e = aNode->point();
    
  float dx1 = fabs( aX - e->center.x );
  float dx2 = fabs( aX - e->beging.x );
  float dx3 = fabs( aX - e->ending.x );
  float dx = std::min<float>(dx1, std::min<float>(dx2, dx3) );

  float dy1 = fabs( aY - e->center.z );
  float dy2 = fabs( aY - e->beging.z );
  float dy3 = fabs( aY - e->ending.z );
  float dy = std::min<float>(dy1, std::min<float>(dy2, dy3) );

  if ( aOrder )
  {
    if ( dx < aEpsilon )
      nearLine = true;

    isInLeftBranch = ( aX < aNode->px() );
  }
  else
  {
    if ( dy < aEpsilon )
      nearLine = true;
    isInLeftBranch = ( aY < aNode->py() );
  }

  //  Check if point has to be added
  if ( nearLine )
  {
    float x     = dx;
    float y     = dy;
    float dist  = sqrt( x*x + y*y );
    if ( dist < aEpsilon )
      aPoints.push_back( aNode->point() );
  }

  //  Recurse 
  if( ( nearLine || isInLeftBranch ) && aNode->hasLeftNode() )
      neighboursRec( aNode->leftNode(), !aOrder, aX, aY, aEpsilon, aPoints ); 

  if( ( nearLine || !isInLeftBranch ) && aNode->hasRightNode() )
      neighboursRec( aNode->rightNode(), !aOrder, aX, aY, aEpsilon, aPoints ); 

}