#ifndef _GAMEBP_MAIN_MENU_HPP
#define _GAMEBP_MAIN_MENU_HPP

#include <vector>
#include <string>
#include "GameBall/TrackBp.hpp"

namespace MainMenu{

  enum eMenuActions{
    eNoAction,
    eFallBack,
    eAccepted
  };

  enum eMenuOptions{
    ePlay, 
    eLoadPreset, 
    eLayout,
    eNumIterations,
    eLongFirst,
    eSeed,
    eLength,
    eAbility,
    eTrackWidth,    // these have to be in order  <-
    eArchDensity,   //                             |
    eBumperDensity, //                             |
    eArchProb,      //                             |
    eBumperProb,    // these have to be in order  >|
    eSavePreset,
    ePlay2, 
    eExit
  };

};

class GameBpMainMenuWidget;

class GameBpMainMenu
{
public:
  GameBpMainMenu( TrackBp::TrackDescription* aTrackDesc, TrackBp::TrackEntityDescription* aEntDesc );
  int index() const;
  int getAction() const;

  bool  selectedPlay() const;
  bool  selectedExit() const;

  void  setIndex( const int aIndex );
  void  setAction( const int aAction );
  void  increaseIndex();
  void  decreaseIndex();

  std::string subWidget( const int aIndex );

  std::vector<std::string> mItems;
  std::vector<GameBpMainMenuWidget*> mItemWidget;

  void left();
  void right();
  bool  redoTrackA();
  bool  redoTrackB();
  bool onSave();

  void  cancelSave();
  void  endSave();
  void  saveFileName( std::string& s );

  std::string mSaveFileName;

private:
  int   mMaxIndex;
  int   mAction;
  int   mIndex;

  bool    mHasSelectedPlay;
  bool    mHasSelectedExit;
  bool    mTrackParametersHaveChangedA;
  bool    mTrackParametersHaveChangedB;
  bool    mOnSave;

  TrackBp::TrackDescription* mTrackDesc;
  TrackBp::TrackEntityDescription* mEntDesc;

  bool load();
  
};

#endif  //_GAMEBP_MAIN_MENU_HPP