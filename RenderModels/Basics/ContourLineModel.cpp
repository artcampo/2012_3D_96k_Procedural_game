#include		"RenderModels\Basics/ContourLineModel.hpp"

/*
  \param    aTime0    time at which it starts
  \param    aTimef    time at which finishes
  \param    aWidth    width of the line
  \param    aSplinePosStart   starting position on spline
  \param    aSplinePosMult    for each unit of time how much moves on the spline
  \param
*/
ContourLineModel::ContourLineModel( float aTime0,  
                                    float aTimef, 
                                    float aWidth,
                                    SplineBezierTMapped* aSplineStrokeWidh,
                                    float aSplinePosStart,
                                    float aSplinePosMult,
                                    D3DXCOLOR aColour,
                                    SplineBezierTMapped* aSpline,
                                    D3DXVECTOR3 aDisplacement
                                    )
{
  mTime0 = aTime0;
  mTimef = aTimef;
  mWidth = aWidth;
  mSplinePosStart = aSplinePosStart;
  mSplinePosMult = aSplinePosMult / aSpline->getNumPoints();
  mColour = aColour;
  mSpline = aSpline;
  mSplineStrokeWidh = aSplineStrokeWidh;

  mBezierTesellationLength = 0.1f;

  mTesellation = 10 * mSpline->getNumArcs();

  mTimeTotal = aTimef - mTime0;
  //  make 40 models for each second
  //mTimeTesellation = int( mTimeTotal / 100 * 40 );
  mTimeTesellation = 10;
  mLines.resize( mTimeTesellation );
  mTimePerStep = mTimeTotal / float(mTimeTesellation);

  for ( int i = 0; i < mTimeTesellation ; ++i)
  {
    mLines[i] = makeModel ( i*mTimePerStep + mTime0, aDisplacement );
  }

}


ContourLineModel::~ContourLineModel()
{
  for ( int i = 0; i < mTimeTesellation ; ++i)
    delete mLines[i];
}

void ContourLineModel::display( float aTime )
{
  
  if ( aTime >= mTime0 && aTime <= mTimef)
  {
    float timePos = (aTime - mTime0) / mTimePerStep;
    int index = int(timePos);
    if (index < 0 || index > mTimeTesellation)
      int b = 0;
    mLines[index]->display();
  }
  
}

LineListModel<ParticleVertex>* ContourLineModel::makeModel( float aTime, D3DXVECTOR3 aDisplacement )
{
  float t = mSplinePosStart + mSplinePosMult * ( aTime - mTime0 );

  std::vector<ParticleVertex> points (mTesellation);
  for ( int i = 0 ; i < mTesellation ; ++i)
  {
    float currentT = t + (mBezierTesellationLength * float(i) / float(mTesellation));
    float strokeT = float(i) / float (mTesellation);
    currentT = fmod( currentT, 1.0f );
    points[i].centerW = mSpline->getPoint(currentT) + aDisplacement;
    points[i].sizeW.x = mWidth * 0.1f*0.5f*(mSplineStrokeWidh->getPoint(currentT)).y;
    points[i].color   = mColour;
  }
  
  return new LineListModel<ParticleVertex>( points, mColour );
  //mLines  = new LineListModel<ParticleVertex>( points, mColour );
}