#include "Assets/Palette.hpp"
#include <iostream>
#include <fstream>

namespace Palette{

std::vector<D3DXCOLOR> loadPalette( std::wstring& aName )
{
  std::ifstream myfile;
  myfile.open (aName.c_str());
  
  int numElems;
  myfile >> numElems;
  
  std::vector<D3DXCOLOR> ret;
  ret.resize(numElems);

  for ( int i = 0 ; i < numElems ; ++i)
  {
    myfile >> ret[i].r;
    myfile >> ret[i].g;
    myfile >> ret[i].b;
    myfile >> ret[i].a;
  }

  myfile.close();
  return ret;
}


};