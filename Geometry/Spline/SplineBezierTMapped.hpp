#ifndef _SPLINEBEZIERTMAPPED_HPP
#define _SPLINEBEZIERTMAPPED_HPP

#include  <vector>

#include  "dx_misc.hpp"
#include  "Project_Dependant/StoredSplines.hpp"
#include "GameBall/IA/Ball_IA _common.hpp"

class SplineBezierTMapped
{
public:


  SplineBezierTMapped( const D3DXVECTOR3* aData, int aNumNodes, int aPointsPerArch = 10 );
	~SplineBezierTMapped();

  D3DXVECTOR3 getSplineNode ( int aIndex );

  D3DXVECTOR3 getPoint            ( float aT );
  D3DXVECTOR3 getPointTangent     ( float aT );
  D3DXVECTOR3 getSecondDerivative ( float aT );

  int   getNumPoints ();
  int   getNumArcs ();
  int   indexOfPoint( int aIndex )  const;

  std::vector<D3DXVECTOR3> getAllCurvePoints ( int aPointsPerArch );
  std::vector<D3DXVECTOR3> getAllCurvePoints ( );

  void getMaxMinMean ( D3DXVECTOR3& aMax, D3DXVECTOR3& aMin, D3DXVECTOR3& aMean );

  //  Functions to correlate free-points and points in the path
  float   nearestPathPoint( const D3DXVECTOR3& aPoint, const int aPrecision );
  bool    advanceAlongPathNunits( const float aUnits, 
                                  const float aT, 
                                  const float aMaxT,
                                  float& aNewT, 
                                  float& aAdvancedUnits,
                                  bool& aMaxAngleSurpassed,
                                  const float aMaxAngle,                                                                                   
                                 const D3DXVECTOR3& aBallPos,
                                 const D3DXVECTOR3& aBallVel,
                                 BallIA::Target& aOutTarget);
   bool advanceAlongPathNunits( const float aUnits, 
                                const float aT, 
                                float& aNewT );

private:

  int   mPointsPerArch;
  int   mNumNodes;       /*!< Number of total nodes */
  std::vector<D3DXVECTOR3>  mNodes;          /*!< Spline nodes */
  int   mNumPoints;
  int   mNumArcs;

  std::vector<float>  mAccLength;      /*!< Accumulated length of node ith, undefined for i=0, i=mNumNodes-1.
                                            mAccLength[i=mNumNodes-2] contains total length */  
  float               mTotalLength;

  void    computeLengths();

  //  for derivatives, minimum value for t
  float mMinT;

  friend class Ball;
};


#endif 
