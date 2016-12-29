#ifndef _GameBP_PlacedEntity_HPP_
#define _GameBP_PlacedEntity_HPP_

class PlacedEntity{

public:
  PlacedEntity( const float aSplineT, const float aBitangentialComponent )
  {
    mSplineT                = aSplineT;
    mBitangentialComponent  = aBitangentialComponent;
  }

public:
  float   mSplineT;  
  float   mBitangentialComponent;

  
  bool  static comparePlacedEntities( const PlacedEntity& a, const PlacedEntity& b )
  {
    return a.mSplineT < b.mSplineT;
  }
};



#endif  //_GameBP_PlacedEntity_HPP_
