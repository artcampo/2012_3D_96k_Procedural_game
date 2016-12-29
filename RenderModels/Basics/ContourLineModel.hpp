#ifndef			_CONTOUR_LINE_MODEL_HPP
#define			_CONTOUR_LINE_MODEL_HPP

#include    <vector>
using namespace std;

#include    "dx_misc.hpp"
#include    "RenderModels\Basics\linelistmodel.h"
#include    "Geometry/Spline/SplineBezierTMapped.hpp"
#include		"Geometry/MiscFunctions.hpp"
using namespace GeometryMiscFunctions;



class ContourLineModel {
public:

  ~ContourLineModel();
  ContourLineModel( float aTime0,  
                    float aTimef, 
                    float aWidth,
                    SplineBezierTMapped* aSplineStrokeWidh,
                    float aSplinePosStart,
                    float aSplinePosMult,
                    D3DXCOLOR aColour,
                    SplineBezierTMapped* aSpline,
                    D3DXVECTOR3 aDisplacement
                    );

  void  display( float aTime  );



private:
  float mTimeTotal;
  float mTimePerStep;
  int   mTimeTesellation;     //one model for each time position
  std::vector<LineListModel<ParticleVertex>*>    mLines;

  float mTime0;
  float mTimef; 
  float mWidth;
  float mSplinePosStart;
  float mSplinePosMult;
  D3DXCOLOR mColour;
  SplineBezierTMapped* mSpline;
  SplineBezierTMapped* mSplineStrokeWidh;

  int mTesellation;
  float mBezierTesellationLength;

  LineListModel<ParticleVertex>* makeModel( float aTime, D3DXVECTOR3 aDisplacement );

};



#endif
