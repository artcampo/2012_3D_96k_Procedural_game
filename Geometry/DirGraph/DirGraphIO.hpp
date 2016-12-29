#ifndef _DIRGRAPH_IO_HPP
#define _DIRGRAPH_IO_HPP

#include "dx_misc.hpp"

namespace DirectedGraph {

  typedef struct{
		int n;
		int sumk;
    D3DXVECTOR3* StoredV;
    int* StoredNumE;
    int* StoredE;
  } StoredDirGraph;

};



#endif // _DIRGRAPH_IO_HPP
