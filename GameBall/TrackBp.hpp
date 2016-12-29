#ifndef _GameTrackBp_HPP_
#define _GameTrackBp_HPP_

#include "Geometry/DxMathHelpers.hpp"
#include "dx_misc.hpp"
#include <vector>

namespace TrackBp{

  
  enum ePatternIns{
    ePatternAB
  };

  enum eEdgeJumpIds{
    eEdgeNormal,
    eEdgeRamp,
    eEdgeNineTurn,
    eEdgeOne80Turn,
    eEdgeLoop,
    eEdgeLateral,
    eEdgeLateralCross,
    eEdgeTwoWayJump,
    eEdgeThreeWayJump,
    eEdgeTwister,
    eEdgeNoGeometry
  };

  enum eTrackFamily{
    eTrackLinear,
    eTrackFall,
    eTrackAbility
  };

  enum eTrackLayout{
    eTrackLayoutPlane,
    eTrackLayoutFall,
    eTrackLayoutSpiral
  };

  class Vertex{
  public:
    Vertex(){};
    Vertex( const D3DXVECTOR3& aX, const D3DXVECTOR3& aT, const D3DXVECTOR3& aN )
    {
      pos = aX;
      tan = aT;
      nor = aN;
      DxMath::cleanZeroes( tan );
      DxMath::cleanZeroes( nor );
    }
    void realignTN ( const D3DXVECTOR3& aNewTangent )
    {
      nor = DxMath::alignToTangents( aNewTangent, tan, nor );
      tan = aNewTangent;
    }
    D3DXVECTOR3 pos;
    D3DXVECTOR3 tan;
    D3DXVECTOR3 nor;
  };

  class EdgeData{
  public:
    EdgeData()
    { 
      mEdgeJumpId = eEdgeNormal;
    };
    EdgeData( const float aDesiredLength,
              const int aEdgeJumpId,
              const unsigned long aSeedForCurves )
    {      
      mDesiredLength  = aDesiredLength;
      mEdgeJumpId     = aEdgeJumpId;
      mSeedForCurves  = aSeedForCurves;
    };
    bool    isModifiable() const
    {
      return (mEdgeJumpId == eEdgeNormal );
    };
    int     jumpId () const
    {
      return mEdgeJumpId;
    };
  public:
    float   mDesiredLength;
    int     mEdgeJumpId;
    unsigned long mSeedForCurves;
  };

  struct TrackDescription{
    int   layout;
    int   iterations;
    int   iterationsAbility;
    int   seed;
    bool  substituteLongerEdgesFirst;
    float curvatureMax;
    float curvatureMin;
    float totalLength;          /*  Multiplier of difficulty must be already applied before use*/
    float numUnitsPerCurve;
    
    int   family;
    bool  lapsed;
    bool  usesIA;
    float trackWidth;           // Width of track
    float trackWidthEntities;   // Width of track where entities can be placed

    D3DXVECTOR3   layoutSpiralInitialBallPosition;
  };

  struct TrackEntityDescription{
    float         archDensity;    /*  Number of units separing two consecutive ones */
    float         archDispersion; /*  Rand over the previous */
    float         bumperDensity;
    float         bumperDispersion;
    float         archHeight;
    float         bumperHeight;
    bool          archTimeUpUsed;
    float         probEdgeWithArch;
    float         probEdgeWithBumper;    
  };

  struct TrackFogBounding{
    float         xzPlaneHeight;
    D3DXVECTOR3   boundingCircCenter;
    float         boundingCircRadius;
  };



};

#endif  //_GameTrackBp_HPP_
