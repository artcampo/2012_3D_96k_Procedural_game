#include		"Geometry/PixelMap.hpp"

PixelMap::PixelMap( D3DXVECTOR2 aBasisOffset, D3DXVECTOR2 aBasisScale, int aX, int aY  )
{
  /*
  mBasisOffset  = D3DXVECTOR2( -360.0f, -220.0f );
  mBasisScale   = D3DXVECTOR2( 1.0f, 1.0f );
  */
  mBasisOffset  = aBasisOffset;
  //mBasisScale   = D3DXVECTOR2( -1.0f, -1.0f );
  mBasisScale   = aBasisScale;

  mXLength      = 0;
  mYLength      = 0;
  mFillColor    = 1;
  mBackColor    = 0;
  
  mXLength      = aX;
  mYLength      = aY;
  resize();
}



void PixelMap::resize()
{
  mMap.resize( mYLength );
  for ( int i = 0; i < mYLength ; ++i )
  {
    mMap[i].resize( mXLength );
  }
}

void PixelMap::clearBackground()
{
  
  for ( int i = 0; i < mYLength ; ++i )
    for ( int j = 0; j < mXLength ; ++j )
    {
      mMap[i][j] = mBackColor;
    }
}

IntCoord  PixelMap::coordMapping ( float aX, float aY )
{
  D3DXVECTOR2   coord = D3DXVECTOR2( aX, aY );
  coord +=  mBasisOffset;
  coord.x *=  mBasisScale.x;
  coord.y *=  mBasisScale.y;
  IntCoord ret;
  ret.x = int ( coord.x );
  ret.y = int ( coord.y );
  return ret;
}

unsigned char PixelMap::get( float aX, float aY )
{
  IntCoord coord = coordMapping( aX, aY );
  if (  coord.x < 0 || coord.x >= mXLength ||
        coord.y < 0 || coord.y >= mYLength )
     return 0;
  else
    return mMap[coord.y][coord.x];
}

unsigned char PixelMap::get( IntCoord aCoord )
{
  if (  aCoord.x < 0 || aCoord.x >= mXLength ||
        aCoord.y < 0 || aCoord.y >= mYLength )
    return mFillColor;
  return mMap[aCoord.y][aCoord.x];
}

void PixelMap::set( float aX, float aY )
{
  IntCoord coord = coordMapping( aX, aY );
  /*
  if (  coord.x < 0 || coord.x >= mXLength ||
        coord.y < 0 || coord.y >= mYLength )
        return;
        */

  mMap[coord.y][coord.x] = mFillColor; 
}

void PixelMap::set( IntCoord aCoord )
{
  mMap[aCoord.y][aCoord.x] = mFillColor; 
}

void  PixelMap::resizeToMaxMin ( float aXmax, float aXmin, float aYmax, float aYmin  )
{
  IntCoord coordMax = coordMapping( aXmax, aYmax );
  IntCoord coordMin = coordMapping( aXmin, aYmin );

  mXLength = coordMax.x + 2;
  mYLength = coordMax.y + 2;
  resize();
  clearBackground();
}

void PixelMap::drawSplineT( SplineBezierTMapped& aSpline, int aNodesPerSegment, int aMode )
{

  for ( int i = 0 ; i < (aSpline.getNumArcs()*aNodesPerSegment) ; ++i)
  {
    D3DXVECTOR3 p = aSpline.getPoint ( float(i)/float(aSpline.getNumArcs()*aNodesPerSegment) );
    /*
    p.x *= -1.0;
    p.y *= -1.0;
    */
    if (aMode == eAxisModeYZ )
      set( p.y, p.z );
    if (aMode ==  eAxisModeXY)
      set( p.x, p.y );
  }
}

void PixelMap::fillRegionsRec ( IntCoord aCoord )
{
  std::stack<IntCoord> stack;

  stack.push( aCoord );
  do {
      IntCoord coord = stack.top();
      stack.pop();
      if ( get(coord) != mFillColor )
      {
        set ( coord );

        coord.x += 1;
        if (coord.x < mXLength && get(coord) != mFillColor )
          stack.push( coord );
        coord.x -= 2;
        if (coord.x >= 0 && get(coord) != mFillColor)
          stack.push ( coord );
        coord.x += 1;
        
        coord.y += 1;
        if (coord.y < mYLength && get(coord) != mFillColor )
          stack.push ( coord );
        coord.y -= 2;
        if (coord.y >= 0 && get(coord) != mFillColor)
          stack.push ( coord );
      }
  } while( !stack.empty());
  
}

/*
void PixelMap::fillRegionsRec ( IntCoord aCoord )
{
  if (  aCoord.x < 0 || aCoord.x >= mXLength ||
        aCoord.y < 0 || aCoord.y >= mYLength )
        return;
  if ( get(aCoord) == mFillColor )
    return;

  set ( aCoord );

  IntCoord coord;
  //try to fill row
  coord = aCoord;
  ++coord.x;
  while( get(coord) != mFillColor && coord.x < (mXLength - 1) )
  {
    set(coord);
    ++coord.x;
  }

 coord = aCoord;
  --coord.x;
  while( get(coord) != mFillColor && coord.x > 1 )
  {
    set(coord);
    --coord.x;
  }

  // recurse
  
  aCoord.x += 1;
  if (get(aCoord) != mFillColor && aCoord.x < mXLength)
    fillRegionsRec ( aCoord );
  aCoord.x -= 2;
  if (get(aCoord) != mFillColor && aCoord.x >= 0)
    fillRegionsRec ( aCoord );
  aCoord.x += 1;
  
  aCoord.y += 1;
  if (get(aCoord) != mFillColor && aCoord.y < mYLength)
    fillRegionsRec ( aCoord );
  aCoord.y -= 2;
  if (get(aCoord) != mFillColor && aCoord.y >= 0)
    fillRegionsRec ( aCoord );
}
*/

/*
  Param: center of flood fill
*/
/*
void PixelMap::fillRegions( float aX, float aY )
{
  IntCoord c = coordMapping( aX, aY );
  fillRegionsRec(c);
}*/

void PixelMap::fillRegions( float aX, float aY )
{
  IntCoord c = coordMapping( aX, aY );
  fillRegionsRec(c);
}

D3DXVECTOR3 PixelMap::getRandomPositionInside()
{
  using namespace Rand;
  RandGen r(42);
  D3DXVECTOR3 ret;

  bool inside = false;
  while (!inside )
  {
    ret = D3DXVECTOR3(
        
        2*530 * r.randFloat_0_1() - 530,
        2*150 * r.randFloat_0_1() - 150,
        0//300 * r.randFloat_0_1(),
        );
    /*
    ret = D3DXVECTOR3(
        0,//300 * r.randFloat_0_1(),
        -14,
        -50
        );*/

    if ( get( ret.x, ret.y ) )
      inside = true;


  }
  return ret;
}

void PixelMap::displayAsQuad()
{

}