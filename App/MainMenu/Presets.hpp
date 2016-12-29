#ifndef _PRESETS_HPP
#define _PRESETS_HPP

#include <vector>
#include <string>

class GameBpMainMenu;

namespace Presets{

int                         numPresets();
std::vector< std::string >  presetList();

void  loadPreset( const std::string& aName, GameBpMainMenu* aPreset  );
void  savePreset( const std::string& aName, GameBpMainMenu* aPreset );
};

#endif  //_PRESETS_HPP