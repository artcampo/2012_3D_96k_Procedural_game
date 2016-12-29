#include "App/MainMenu/GameBpMainMenu.hpp"
#include "App/MainMenu/GameBpMainMenuWidget.hpp"
#include "App/MainMenu/Presets.hpp"

#include <sstream>

GameBpMainMenu::GameBpMainMenu( TrackBp::TrackDescription* aTrackDesc, TrackBp::TrackEntityDescription* aEntDesc )
  : mHasSelectedPlay(false),
    mHasSelectedExit(false),
    mTrackParametersHaveChangedA(true),
    mTrackParametersHaveChangedB(true),
    mOnSave(false),
    mTrackDesc(aTrackDesc),
    mEntDesc(aEntDesc)
{
  mMaxIndex = MainMenu::eExit + 1;
  mItems.push_back( std::string("Play") );
  mItemWidget.push_back( new GameBpMainMenuWidget() );

  mItems.push_back( std::string("Load preset") );
  if ( Presets::numPresets() == 0)
  {
    mItemWidget.push_back( new GameBpMainMenuWidget( 1, 0, 1, true) );
    std::vector< std::string > s(1);
    s[0] = std::string("None");    
    mItemWidget[MainMenu::eLoadPreset]->setList(s);
  }
  else
  {
    mItemWidget.push_back( new GameBpMainMenuWidget( Presets::numPresets(), 0, 1, true) );
    mItemWidget[MainMenu::eLoadPreset]->setList( Presets::presetList() );
  }

  mItems.push_back( std::string("Layout") );  
  mItemWidget.push_back( new GameBpMainMenuWidget( 3, 0, 3, true) );
  std::vector< std::string > s(3);
  s[0] = std::string("Plane");    
  s[1] = std::string("Fall");    
  s[2] = std::string("Spiral");    
  mItemWidget[MainMenu::eLayout]->setList(s);

  mItems.push_back( std::string("Iterations") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 40, 10, 50) );

  mItems.push_back( std::string("Substitution rule") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 2, 0, 1, true) );
  std::vector< std::string > s1(2);
  s1[0] = std::string("Randomly");
  s1[1] = std::string("Longer edges");
  mItemWidget[MainMenu::eLongFirst]->setList(s1);

  mItems.push_back( std::string("Seed") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 40, 42, 1235 ) );

  mItems.push_back( std::string("Length") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 30, 300, 2000 ) ); //800, 2000

  mItems.push_back( std::string("Ability pieces") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 50, 0, 50) );

  mItems.push_back( std::string("Track Width") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 20, 3, 15 ) );

  mItems.push_back( std::string("Arch density") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 30, 25, 60 ) );

  mItems.push_back( std::string("Bumper density") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 30, 15, 60 ) );

  mItems.push_back( std::string("Edges with archs") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 20, 0, 1 ) );

  mItems.push_back( std::string("Edges with bumpers") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 20, 0, 1 ) );

  mItems.push_back( std::string("Save preset") );
  mItemWidget.push_back( new GameBpMainMenuWidget( 1, 0, 1 ) );

  mItems.push_back( std::string("Play") );
  mItemWidget.push_back( new GameBpMainMenuWidget() );

  mItems.push_back( std::string("Exit") );
  mItemWidget.push_back( new GameBpMainMenuWidget() );

  mIndex = 0;  
}

bool  GameBpMainMenu::selectedPlay() const
{
  return mHasSelectedPlay;
}

bool  GameBpMainMenu::selectedExit() const
{
  return mHasSelectedExit;
}


int GameBpMainMenu::index() const
{
  return mIndex;
}
int GameBpMainMenu::getAction() const
{
  return mAction;
}
void  GameBpMainMenu::setAction( const int aAction )
{
  if ( aAction == MainMenu::eFallBack || aAction == MainMenu::eNoAction)
    return;

  if ( mIndex == MainMenu::ePlay || mIndex == MainMenu::ePlay2)
    mHasSelectedPlay = true;

  if ( mIndex == MainMenu::eExit )
    mHasSelectedExit = true;
  
  if ( aAction == MainMenu::eAccepted
    && mIndex == MainMenu::eSavePreset )
  {
    mSaveFileName = std::string("");
    mOnSave = true;
  }
}

void  GameBpMainMenu::setIndex( const int aIndex )
{
  mIndex = aIndex;
}


void  GameBpMainMenu::decreaseIndex()
{
  if( mIndex > 0 ) --mIndex;
}

void  GameBpMainMenu::increaseIndex()
{
  if( mIndex < (mMaxIndex - 1) ) ++mIndex;
}


std::string  GameBpMainMenu::subWidget( const int aIndex )
{
  /*
  std::string text("");
  if ( mIndex == MainMenu::ePlay || mIndex == MainMenu::eExit )
    return text;
*/
  return mItemWidget[aIndex]->text();

}

void GameBpMainMenu::left()
{
  if ( mIndex == MainMenu::ePlay || mIndex == MainMenu::eExit )
    return;

  mItemWidget[mIndex]->decrease();

  if ( load() )
    return;     

  if ( ! ( mIndex >= MainMenu::eTrackWidth &&  mIndex <= MainMenu::eBumperProb ) )
  {
    if ( mIndex == MainMenu::eAbility )
      mTrackParametersHaveChangedB = true;
    else
      mTrackParametersHaveChangedA = true;
  }
}

void GameBpMainMenu::right()
{  
  if ( mIndex == MainMenu::ePlay || mIndex == MainMenu::eExit )
    return;
    
  int v = mItemWidget[mIndex]->value();

  mItemWidget[mIndex]->increase();

  if ( v == mItemWidget[mIndex]->value() ) return;

  if ( load() )
    return;    

  if ( ! ( mIndex >= MainMenu::eTrackWidth &&  mIndex <= MainMenu::eBumperProb ) )
  {
    if ( mIndex == MainMenu::eAbility )
      mTrackParametersHaveChangedB = true;
    else
      mTrackParametersHaveChangedA = true;
  }
}

bool  GameBpMainMenu::redoTrackA()
{
  if ( mTrackParametersHaveChangedA )
  {
    mTrackParametersHaveChangedA = false;
    mTrackParametersHaveChangedB = true;
    return true;
  }
  else
    return false;
}

bool  GameBpMainMenu::redoTrackB()
{
  if ( mTrackParametersHaveChangedB )
  {
    mTrackParametersHaveChangedB = false;
    return true;
  }
  else
    return false;
}

bool GameBpMainMenu::load()
{
  if ( mIndex == MainMenu::eLoadPreset )
  {
    std::string s = mItemWidget[MainMenu::eLoadPreset]->text();
    if ( s.compare( std::string("None")) )
    { 
      Presets::loadPreset( s, this );
      mTrackParametersHaveChangedA = true;
    }
    return true;  
  }
  return false;  
}

bool GameBpMainMenu::onSave()
{
  return mOnSave;
}

void  GameBpMainMenu::cancelSave()
{
  mOnSave = false;
}

void  GameBpMainMenu::endSave()
{

  Presets::savePreset( mSaveFileName, this  );
  mOnSave = false;
}

void  GameBpMainMenu::saveFileName( std::string& s )
{
  mSaveFileName = s;
}