#ifndef _SPLINETMAPPED_HPP
#define _SPLINETMAPPED_HPP

// TODO: mixed class! offers two functionalities!!

#include  <vector>

#include  "dx_misc.hpp"
#include  "Project_Dependant/StoredSplines.hpp"

class SplineTMapped
{
public:
  SplineTMapped( D3DXVECTOR3* aData, int aNumNodes );
  SplineTMapped( float* aData, int aNumNodes );
	~SplineTMapped();

  D3DXVECTOR3 getPoint ( float t );
  D3DXVECTOR3 getPointNormal ( float t );
  int   getNumNodes ();

  void getMaxMinMean ( D3DXVECTOR3& aMax, D3DXVECTOR3& aMin, D3DXVECTOR3& aMean );

public:
  SplineTMapped::SplineTMapped( D3DXVECTOR3* aData, 
                              int aNumNodes,
                              float aTime0,
                              float aTimeF,
                              int aRepeatMode);
  D3DXVECTOR3 getPointViaTime ( float aTime );

  enum eRepeatModes{
    Clamp,
    Mod
  };
private:
  float mTime0;
  float mTimeF;
  int   mRepeatMode;
private:

  int                 mNumNodes;       /*!< Number of total nodes */
  std::vector<D3DXVECTOR3>  mNodes;          /*!< Spline nodes */

  std::vector<float>  mAccLength;      /*!< Accumulated length of node ith, undefined for i=0, i=mNumNodes-1.
                                            mAccLength[i=mNumNodes-2] contains total length */  
  float               mTotalLength;

  void    computeLengths();
};


#endif 
