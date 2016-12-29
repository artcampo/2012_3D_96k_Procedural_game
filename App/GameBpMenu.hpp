#ifndef _GAMEBP_MENU_HPP
#define _GAMEBP_MENU_HPP

#include <vector>
#include <string>

namespace Menu{

  enum eMenuActions{
    eNoAction,
    eFallBack,
    eAccepted
  };

};

class GameBpMenu
{
public:
  GameBpMenu( std::vector<std::string> aItems );
  int index() const;
  int getAction() const;

  void  setIndex( const int aIndex );
  void  setAction( const int aAction );
  void  increaseIndex();
  void  decreaseIndex();

  std::vector<std::string> mItems;
private:
  int   mMaxIndex;
  int   mAction;
  int   mIndex;
  
};

#endif