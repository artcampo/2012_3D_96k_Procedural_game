#ifndef _PALETTE_HPP_
#define _PALETTE_HPP_

#include "dx_misc.hpp"
#include <vector>

namespace Palette{

std::vector<D3DXCOLOR> loadPalette( std::wstring& aName );


};

#endif