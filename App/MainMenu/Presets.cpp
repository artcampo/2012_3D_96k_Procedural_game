#include "App/MainMenu/Presets.hpp"
#include "App/MainMenu/GameBpMainMenu.hpp"
#include "App/MainMenu/GameBpMainMenuWidget.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Presets{

int   numPresets()
{
  return presetList().size();
}

std::vector< std::string >  presetList()
{
  using namespace std;
  std::vector< std::string > list;
  
  ifstream fileStd( "presets/list.txt", fstream::in  );
  if ( fileStd.is_open() )
    while ( !fileStd.eof() )
    {
       std::string s;
       fileStd >> s;
       if ( !s.empty() )
        list.push_back(s);
    }
  return list;
}

void loadPreset( const std::string& aName, GameBpMainMenu* aPreset  )
{
  using namespace std;
  std::stringstream ss;
  ss << std::string("presets/");
  ss << aName;
  ifstream fileStd( ss.str().c_str(), fstream::in );  

  for ( int i = MainMenu::eLayout ; i <= MainMenu::eBumperProb ; ++i )
  {
    int index;
    fileStd >> index;
    aPreset->mItemWidget[i]->setIndex(index);
  }
  fileStd.close();
}

void savePreset( const std::string& aName, GameBpMainMenu* aPreset  )
{
  using namespace std;
  
  std::stringstream ss;
  ss << std::string("presets/");
  ss << aName;
  ofstream fileStd( ss.str().c_str(), fstream::out );  

  for ( int i = MainMenu::eLayout ; i <= MainMenu::eBumperProb ; ++i )
  {
    int index = aPreset->mItemWidget[i]->index();
    fileStd << index << std::endl;
  }
  fileStd.close();

  {    
    ofstream fileStd( "presets/list.txt", fstream::out|| fstream::trunc );

    if ( !aPreset->mItemWidget[MainMenu::eLoadPreset]->mList[0].compare( std::string("None") ) )
    {
      aPreset->mItemWidget[MainMenu::eLoadPreset]->mList.clear();
      aPreset->mItemWidget[MainMenu::eLoadPreset]->mSteps = 0;
    }

    aPreset->mItemWidget[MainMenu::eLoadPreset]->mList.push_back( aPreset->mSaveFileName );
    aPreset->mItemWidget[MainMenu::eLoadPreset]->mSteps++;
    for ( int i = 0; i < aPreset->mItemWidget[MainMenu::eLoadPreset]->mList.size() ; ++i)
    {
      fileStd << aPreset->mItemWidget[MainMenu::eLoadPreset]->mList[i] << std::endl;
      
    }
    fileStd.close();
  }

}
};


