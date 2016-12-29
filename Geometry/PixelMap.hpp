#ifndef _PIXELMAP_HPP_
#define _PIXELMAP_HPP_

#include  <vector>
#include  <stack>
#include  "dx_misc.hpp"
#include  "Geometry/Spline/SplineBezierTMapped.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

struct IntCoord{
  int   x;
  int   y;
};



class PixelMap
{
public:
  enum eAxisModes{
    eAxisModeYZ,
    eAxisModeXY
  };

  public:
    PixelMap( D3DXVECTOR2 aBasisOffset, D3DXVECTOR2 aBasisScale , int aX, int aY  );
    D3DXVECTOR3 getRandomPositionInside();
 
    void  resizeToMaxMin ( float aXmax, float aXmin, float aYmax, float aYmin  );

    void              set( float aX, float aY );
    void              set( IntCoord aCoord );
    unsigned char     get( float aX, float aY );
    unsigned char     get( IntCoord aCoord );

    //void    fillRegions( float aX, float aY );
    void    fillRegions( float aX, float aY );
    void    drawSplineT( SplineBezierTMapped& aSpline, int aNodesPerSegment, int aMode );

    void      displayAsQuad();

  private:
    IntCoord  coordMapping ( float aX, float aY );
    void      resize();
    void      clearBackground();
    void      fillRegionsRec ( IntCoord aCoord );

  private:
    std::vector< std::vector<unsigned char> >  mMap;
    int           mXLength;
    int           mYLength;
    D3DXVECTOR2   mBasisOffset;
    D3DXVECTOR2   mBasisScale;

    unsigned char mFillColor;
    unsigned char mBackColor;
};


//---------------------------------------------
// EXTERN FUNCTIONS
//---------------------------------------------

void  dx_3dPerpendicular ( D3DXVECTOR3* n, D3DXVECTOR3* v );

#endif