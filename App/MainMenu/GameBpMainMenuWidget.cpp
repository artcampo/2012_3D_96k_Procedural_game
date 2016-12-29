#include "App/MainMenu/GameBpMainMenuWidget.hpp"
#include <sstream>

GameBpMainMenuWidget::GameBpMainMenuWidget( int aSteps, float aMin, float aMax, const bool aIsList  )
  : mSteps(aSteps),
    mMin(aMin),
    mMax(aMax),
    mIsList(aIsList)
{
  mValue = 0;
}


std::string  GameBpMainMenuWidget::text() const
{
  std::string text("");
  if ( mSteps == 0 )
    return text;

  if ( mIsList )
    return mList[mValue];

  std::stringstream ss;
  ss << mValue;
  return ss.str(); 
}

void GameBpMainMenuWidget::increase()
{
  if ( mValue < mSteps - 1 ) ++mValue;
}

void GameBpMainMenuWidget::decrease()
{
  if ( mValue > 0) --mValue;
}

float GameBpMainMenuWidget::value()
{
  return mMin + ((mMax - mMin)*float(mValue))/float(mSteps);
}

void  GameBpMainMenuWidget::setList( const std::vector< std::string >& aList)
{
  mList = aList;
}

int   GameBpMainMenuWidget::index() const
{
  return mValue;
}