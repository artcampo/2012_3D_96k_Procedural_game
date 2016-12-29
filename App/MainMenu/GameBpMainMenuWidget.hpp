#ifndef _GAMEBP_MAIN_MENU_WIDGET_HPP
#define _GAMEBP_MAIN_MENU_WIDGET_HPP

#include <vector>
#include <string>

class GameBpMainMenuWidget
{
public:
  GameBpMainMenuWidget(){mSteps = 0;};
  GameBpMainMenuWidget( int aSteps, float aMin, float aMax, const bool aIsList = false );

  std::string text() const;
  
  void  increase();
  void  decrease();
  float value();

  int   index() const;
  void  setIndex( const int aVal ) { mValue = aVal; };
  void  setList( const std::vector< std::string >& aList);
 
  
private: 
  bool  mIsList;
  int   mValue;
  
  float mMin;
  float mMax;
public:
  std::vector< std::string >  mList;
  int   mSteps;


  
};

#endif  //_GAMEBP_MAIN_MENU_WIDGET_HPP