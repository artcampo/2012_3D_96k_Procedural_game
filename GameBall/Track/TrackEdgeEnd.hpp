#ifndef _GameTrackEdgeEnd_HPP_
#define _GameTrackEdgeEnd_HPP_

#include "dx_misc.hpp"
#include <vector>

#include "Geometry\Spline\SplineBezierTMapped.hpp"
#include "RenderModels\DxMeshModel.hpp"
#include "RenderModels\DxLodModel.hpp"
#include "Physics/MeshCol.hpp"
#include "Geometry/Loft.hpp"
#include "GameBall/TrackBp.hpp"

class TrackEdgeEnd {
public:  
  ~TrackEdgeEnd();
  TrackEdgeEnd ( const D3DXVECTOR3& mPoints, 
              const std::vector<D3DXVECTOR3>& mBezierPointsSlice,
              const D3DXVECTOR3& aTangent,
              const D3DXVECTOR3& aNormal );

  void  generateLoftSurface ();
  DxLodModel<VertPNTUV>*   model() const;  
  std::vector<MeshCol*>           meshColTri() const;



  //  for convex meshes
  MeshCol*                        meshCol() const;

private:
  //static const int    mTesselationPerUnit = 2;
  static const int    mTesselationPerUnit = 6;
private:

private:
  SplineBezierTMapped*  mSplineSlice;

  D3DXVECTOR3           mTangent;
  D3DXVECTOR3           mNormal;



};


#endif  //_GameTrackEdgeEnd_HPP_
