#include "GameBpMenu.hpp"

GameBpMenu::GameBpMenu( std::vector<std::string> aItems )
{
  mMaxIndex = aItems.size();
  mItems = aItems;
  mIndex = 0;
}

int GameBpMenu::index() const
{
  return mIndex;
}
int GameBpMenu::getAction() const
{
  return mAction;
}
void  GameBpMenu::setAction( const int aAction )
{
  mAction = aAction;
}

void  GameBpMenu::setIndex( const int aIndex )
{
  mIndex = aIndex;
}


void  GameBpMenu::decreaseIndex()
{
  if( mIndex > 0 ) --mIndex;
}

void  GameBpMenu::increaseIndex()
{
  if( mIndex < (mMaxIndex - 1) ) ++mIndex;
}

