#include  "GameBall/GameAssets/TrackOutPieces.hpp"
namespace StoredDirGraphrt1 {

D3DXVECTOR3 StoredV[9] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.275705 , 0 , -0.211417 ) ,
  D3DXVECTOR3(0.479491 , -0.05 , -0.35 ) ,
  D3DXVECTOR3(0.65 , 0 , -0.55 ) ,
  D3DXVECTOR3(0.783804 , 0 , -0.516513 ) ,
  D3DXVECTOR3(0.842205 , 0 , -0.0227182 ) ,
  D3DXVECTOR3(0.237541 , 0 , 0.1 ) ,
  D3DXVECTOR3(0.468453 , 0.0662075 , 0.203961 ) ,
  D3DXVECTOR3(0.960864 , 0 , -0.0181454 ) 
};

int StoredNumE[9] = {
2,1,1,
1,1,1,
1,1,0
};

int StoredE[9] = {
1,6,2,
3,4,5,
8,7,5
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph rt1 = { 
  9,
  9,
  &StoredDirGraphrt1::StoredV[0],
  &StoredDirGraphrt1::StoredNumE[0],
  &StoredDirGraphrt1::StoredE[0] 
};


namespace StoredDirGraphrt2 {

D3DXVECTOR3 StoredV[24] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.275705 , 0 , -0.211417 ) ,
  D3DXVECTOR3(0.479491 , 0 , -0.35 ) ,
  D3DXVECTOR3(0.65 , 0 , -0.55 ) ,
  D3DXVECTOR3(0.783804 , 0 , -0.516513 ) ,
  D3DXVECTOR3(0.842205 , 0 , -0.0227182 ) ,
  D3DXVECTOR3(0.0955603 , -0.0095613 , -0.00228181 ) ,
  D3DXVECTOR3(0.35609 , 0 , 0.203961 ) ,
  D3DXVECTOR3(0.960864 , 0 , -0.0181454 ) ,
  D3DXVECTOR3(0.15 , 0 , 0.3 ) ,
  D3DXVECTOR3(0.45 , -0.0570376 , 0.50016 ) ,
  D3DXVECTOR3(0.7 , 0 , 0.65 ) ,
  D3DXVECTOR3(1 , 0.033145 , 0.477028 ) ,
  D3DXVECTOR3(0.965948 , 0 , 0.262028 ) ,
  D3DXVECTOR3(0.8 , 0 , 0.2 ) ,
  D3DXVECTOR3(0.9 , 0 , 0 ) ,
  D3DXVECTOR3(0.65 , 0.0630531 , 0.35 ) ,
  D3DXVECTOR3(0.45 , 0 , 0.3 ) ,
  D3DXVECTOR3(0.55 , 0 , 0.05 ) ,
  D3DXVECTOR3(0.25 , -0.109885 , -0.45 ) ,
  D3DXVECTOR3(0.45 , -0.112339 , -0.15 ) ,
  D3DXVECTOR3(0.6 , -0.0879027 , -0.4 ) ,
  D3DXVECTOR3(0.95 , 0 , -0.55 ) ,
  D3DXVECTOR3(0.732009 , 0 , -0.35 ) 
};

int StoredNumE[24] = {
1,2,1,
1,1,1,
3,1,0,
1,1,2,
1,1,1,
1,1,1,
1,1,1,
1,1,1
};

int StoredE[27] = {
6,2,19,
3,4,23,
8,7,1,
9,18,10,
11,12,16,
13,14,15,
5,17,18,
5,20,21,
22,23,5
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph rt2 = { 
  24,
  27,
  &StoredDirGraphrt2::StoredV[0],
  &StoredDirGraphrt2::StoredNumE[0],
  &StoredDirGraphrt2::StoredE[0] 
};
namespace StoredDirGraphsp1 {

D3DXVECTOR3 StoredV[5] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.2 , 0 , 0.00252904 ) ,
  D3DXVECTOR3(0.474769 , -0.103119 , -0.00284569 ) ,
  D3DXVECTOR3(0.805352 , 0 , 0 ) ,
  D3DXVECTOR3(0.95 , 0 , 0 ) 
};

int StoredNumE[5] = {
1,1,1,
1,0
};

int StoredE[4] = {
1,2,3,
4
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph sp1 = { 
  5,
  4,
  &StoredDirGraphsp1::StoredV[0],
  &StoredDirGraphsp1::StoredNumE[0],
  &StoredDirGraphsp1::StoredE[0] 
};

namespace StoredDirGraphsp2 {

D3DXVECTOR3 StoredV[5] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.2 , 0 , -0.181347 ) ,
  D3DXVECTOR3(0.493738 , 0 , -0.272476 ) ,
  D3DXVECTOR3(0.805352 , 0 , -0.117417 ) ,
  D3DXVECTOR3(0.95 , 0 , 0 ) 
};

int StoredNumE[5] = {
1,1,1,
1,0
};

int StoredE[4] = {
1,2,3,
4
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph sp2 = { 
  5,
  4,
  &StoredDirGraphsp2::StoredV[0],
  &StoredDirGraphsp2::StoredNumE[0],
  &StoredDirGraphsp2::StoredE[0] 
};


namespace StoredDirGraphsp3 {

D3DXVECTOR3 StoredV[3] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.5 , 0 , -0.1 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) 
};

int StoredNumE[3] = {
1,1,0
};

int StoredE[2] = {
1,2
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph sp3 = { 
  3,
  2,
  &StoredDirGraphsp3::StoredV[0],
  &StoredDirGraphsp3::StoredNumE[0],
  &StoredDirGraphsp3::StoredE[0] 
};

////////////////////////////////////////////////////////////
//   lp1
////////////////////////////////////////////////////////////

namespace StoredDirGraphlp1 {

D3DXVECTOR3 StoredV[18] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.05 , 0 , -0.25 ) ,
  D3DXVECTOR3(0.1 , 0 , -0.5 ) ,
  D3DXVECTOR3(0.3 , 0.051917 , -0.8079 ) ,
  D3DXVECTOR3(0.5 , 0 , -0.639295 ) ,
  D3DXVECTOR3(0.2 , 0 , 0.25 ) ,
  D3DXVECTOR3(0.4 , -0.0742875 , 0.6 ) ,
  D3DXVECTOR3(0.6 , 0 , 0.507918 ) ,
  D3DXVECTOR3(0.684468 , 0 , 0.247906 ) ,
  D3DXVECTOR3(0.35 , 0 , 0.2 ) ,
  D3DXVECTOR3(0.55 , 0 , 0.316908 ) ,
  D3DXVECTOR3(0.554 , 0.063821 , 0.104076 ) ,
  D3DXVECTOR3(0.888654 , 0 , 0.1 ) ,
  D3DXVECTOR3(0.25 , 0 , -0.25 ) ,
  D3DXVECTOR3(0.393658 , -0.0697962 , -0.4 ) ,
  D3DXVECTOR3(0.631986 , 0 , -0.507917 ) ,
  D3DXVECTOR3(0.726308 , 0 , -0.177546 ) ,
  D3DXVECTOR3(0.997073 , 0 , 0 ) 
};

int StoredNumE[18] = {
4,1,1,
1,1,1,
1,1,1,
1,1,1,
1,1,1,
1,1,0
};

int StoredE[20] = {
1,5,9,
13,2,3,
4,11,6,
7,8,16,
10,11,12,
17,14,15,
16,17
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph lp1 = { 
  18,
  20,
  &StoredDirGraphlp1::StoredV[0],
  &StoredDirGraphlp1::StoredNumE[0],
  &StoredDirGraphlp1::StoredE[0] 
};

////////////////////////////////////////////////////////////
//   lp3
////////////////////////////////////////////////////////////

namespace StoredDirGraphlp3 {

D3DXVECTOR3 StoredV[23] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.2 , 0 , 0.2 ) ,
  D3DXVECTOR3(0.15 , 0.0679963 , 0.7 ) ,
  D3DXVECTOR3(0.35 , 0 , 1.25 ) ,
  D3DXVECTOR3(0.65 , 0.128079 , 0.646551 ) ,
  D3DXVECTOR3(0.2 , -0.126635 , -0.45 ) ,
  D3DXVECTOR3(0.60056 , 0.15253 , -0.119979 ) ,
  D3DXVECTOR3(0.4 , 0 , 0.6 ) ,
  D3DXVECTOR3(0.7 , 0 , 1.15 ) ,
  D3DXVECTOR3(0.8 , 0 , 0.7 ) ,
  D3DXVECTOR3(0.350533 , -0.150588 , -0.0355817 ) ,
  D3DXVECTOR3(0.3 , 0 , -0.9 ) ,
  D3DXVECTOR3(0.7 , 0 , -1.38283 ) ,
  D3DXVECTOR3(1.13177 , 0 , -0.713627 ) ,
  D3DXVECTOR3(0.5 , 0 , -0.4 ) ,
  D3DXVECTOR3(0.7 , 0 , -0.3 ) ,
  D3DXVECTOR3(0.500829 , 0.130554 , -0.693443 ) ,
  D3DXVECTOR3(0.575567 , -0.10587 , -1.12209 ) ,
  D3DXVECTOR3(0.811752 , 0 , -0.15 ) ,
  D3DXVECTOR3(0.65 , -0.0671697 , 0.15 ) ,
  D3DXVECTOR3(0.572221 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(1.36972 , 0 , -0.5 ) 
};

int StoredNumE[23] = {
2,1,1,
1,1,2,
1,1,1,
1,1,2,
1,1,1,
1,1,1,
1,1,1,
0,1
};

int StoredE[25] = {
1,5,2,
3,4,19,
6,11,7,
8,9,10,
19,12,22,
16,14,15,
18,17,13,
20,20,21,
18
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph lp3 = { 
  23,
  25,
  &StoredDirGraphlp3::StoredV[0],
  &StoredDirGraphlp3::StoredNumE[0],
  &StoredDirGraphlp3::StoredE[0] 
};

namespace StoredPieces{


////////////////////////////////////////////////////////////
//   m0
////////////////////////////////////////////////////////////

namespace StoredDirGraphm0 {

D3DXVECTOR3 StoredV[12] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.1 , 0 , 0 ) ,
  D3DXVECTOR3(0.35 , -0.1 , -0.1 ) ,
  D3DXVECTOR3(0.7 , 0.05 , 0.1 ) ,
  D3DXVECTOR3(0.95 , 0 , 0 ) ,
  D3DXVECTOR3(0.35 , 0.15 , 0.1 ) ,
  D3DXVECTOR3(0.75 , -0.05 , -0.1 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.15 , 0 , 0.025 ) ,
  D3DXVECTOR3(0.15 , 0 , -0.025 ) ,
  D3DXVECTOR3(0.85 , 0 , 0.05 ) ,
  D3DXVECTOR3(0.85 , 0 , -0.05 ) 
};

int StoredNumE[12] = {
1,2,1,
1,1,1,
1,0,1,
1,1,1
};

int StoredE[12] = {
1,8,9,
3,10,7,
6,11,5,
2,4,4
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph m0 = { 
  12,
  12,
  &StoredDirGraphm0::StoredV[0],
  &StoredDirGraphm0::StoredNumE[0],
  &StoredDirGraphm0::StoredE[0] 
};
////////////////////////////////////////////////////////////
//   m1
////////////////////////////////////////////////////////////



namespace StoredDirGraphp1 {

D3DXVECTOR3 StoredV[12] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.15 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , -0.15 ) ,
  D3DXVECTOR3(0.5 , 0 , -0.95 ) ,
  D3DXVECTOR3(0.7 , 0 , -0.15 ) ,
  D3DXVECTOR3(0.85 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , 0.25 ) ,
  D3DXVECTOR3(0.55 , 0 , 0.35 ) ,
  D3DXVECTOR3(0.7 , 0 , 0.6 ) ,
  D3DXVECTOR3(0.95 , 0 , 0.45 ) ,
  D3DXVECTOR3(0.5 , 0 , 0.2 ) 
};

int StoredNumE[12] = {
1,2,1,
1,1,1,
0,1,1,
1,1,1
};

int StoredE[12] = {
1,2,7,
3,4,5,
6,8,9,
10,11,5
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph p1 = { 
  12,
  12,
  &StoredDirGraphp1::StoredV[0],
  &StoredDirGraphp1::StoredNumE[0],
  &StoredDirGraphp1::StoredE[0] 
};

////////////////////////////////////////////////////////////
//   p2
////////////////////////////////////////////////////////////

namespace StoredDirGraphp2 {

D3DXVECTOR3 StoredV[9] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.15 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , -0.15 ) ,
  D3DXVECTOR3(0.5 , 0 , -0.4 ) ,
  D3DXVECTOR3(0.7 , 0 , -0.15 ) ,
  D3DXVECTOR3(0.85 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , 0.25 ) ,
  D3DXVECTOR3(0.45 , 0 , 0.25 ) 
};

int StoredNumE[9] = {
1,2,1,
1,1,1,
0,1,1
};

int StoredE[9] = {
1,2,7,
3,4,5,
6,8,5
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph p2 = { 
  9,
  9,
  &StoredDirGraphp2::StoredV[0],
  &StoredDirGraphp2::StoredNumE[0],
  &StoredDirGraphp2::StoredE[0] 
};


////////////////////////////////////////////////////////////
//   p4
////////////////////////////////////////////////////////////

namespace StoredDirGraphp4 {

D3DXVECTOR3 StoredV[10] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.15 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , -0.45 ) ,
  D3DXVECTOR3(0.5 , 0 , 0 ) ,
  D3DXVECTOR3(0.7 , 0 , -0.35 ) ,
  D3DXVECTOR3(0.85 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , 0.1 ) ,
  D3DXVECTOR3(0.45 , 0 , 0.55 ) ,
  D3DXVECTOR3(0.7 , 0 , 0.1 ) 
};

int StoredNumE[10] = {
1,2,1,
1,1,1,
0,1,1,
1
};

int StoredE[10] = {
1,2,7,
3,4,5,
6,8,9,
5
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph p4 = { 
  10,
  10,
  &StoredDirGraphp4::StoredV[0],
  &StoredDirGraphp4::StoredNumE[0],
  &StoredDirGraphp4::StoredE[0] 
};


////////////////////////////////////////////////////////////
//   p3
////////////////////////////////////////////////////////////

namespace StoredDirGraphp3 {

D3DXVECTOR3 StoredV[16] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.15 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , -0.15 ) ,
  D3DXVECTOR3(-0.2 , 0 , -0.6 ) ,
  D3DXVECTOR3(0.303436 , 0 , -0.527963 ) ,
  D3DXVECTOR3(0.85 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.3 , 0 , 0.146656 ) ,
  D3DXVECTOR3(-0.0691942 , 0 , 0.474955 ) ,
  D3DXVECTOR3(0.0916035 , 0 , 1.04033 ) ,
  D3DXVECTOR3(0.987557 , 0 , 0.931339 ) ,
  D3DXVECTOR3(0.743109 , 0 , 0.100653 ) ,
  D3DXVECTOR3(0.25 , 0 , 0.7 ) ,
  D3DXVECTOR3(0.3 , 0 , 0.6 ) ,
  D3DXVECTOR3(0.262881 , 0 , -0.95 ) ,
  D3DXVECTOR3(0.646694 , 0 , -0.6 ) 
};

int StoredNumE[16] = {
1,2,1,
2,1,1,
0,1,1,
2,1,1,
1,1,1,
1
};

int StoredE[18] = {
1,2,7,
3,4,14,
5,6,8,
9,10,12,
11,5,13,
11,15,5
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph p3 = { 
  16,
  18,
  &StoredDirGraphp3::StoredV[0],
  &StoredDirGraphp3::StoredNumE[0],
  &StoredDirGraphp3::StoredE[0] 
};
////////////////////////////////////////////////////////////
//   twoLevel
////////////////////////////////////////////////////////////

namespace StoredDirGraphtwoLevel {

D3DXVECTOR3 StoredV[14] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.123623 , -0.00315338 , -0.132702 ) ,
  D3DXVECTOR3(0.781189 , -0.0554 , -0.252006 ) ,
  D3DXVECTOR3(0.836773 , -0.0003106 , -0.108317 ) ,
  D3DXVECTOR3(0.925537 , -0.00185108 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.05 , -0.000690103 , 0.000490189 ) ,
  D3DXVECTOR3(0.230483 , -0.056482 , -0.210544 ) ,
  D3DXVECTOR3(0.170139 , -0.000718817 , -0.000488162 ) ,
  D3DXVECTOR3(0.340662 , 0.0685149 , 0.212827 ) ,
  D3DXVECTOR3(0.65 , 0.070765 , 0.240664 ) ,
  D3DXVECTOR3(0.764072 , 0.00318108 , 0.0915822 ) ,
  D3DXVECTOR3(0.834814 , -0.000724465 , 0.000146724 ) ,
  D3DXVECTOR3(0.240713 , 0.0265718 , 0.15 ) 
};

int StoredNumE[14] = {
1,1,1,
1,1,0,
2,1,1,
1,1,1,
1,1
};

int StoredE[14] = {
6,7,3,
4,5,1,
8,2,13,
10,11,12,
4,9
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph twoLevel = { 
  14,
  14,
  &StoredDirGraphtwoLevel::StoredV[0],
  &StoredDirGraphtwoLevel::StoredNumE[0],
  &StoredDirGraphtwoLevel::StoredE[0] 
};

////////////////////////////////////////////////////////////
//   lp2
////////////////////////////////////////////////////////////

namespace StoredDirGraphlp2 {

D3DXVECTOR3 StoredV[24] = {
  D3DXVECTOR3(0 , 0.000235319 , 0 ) ,
  D3DXVECTOR3(0.15 , 0.0710289 , -0.655988 ) ,
  D3DXVECTOR3(0.282872 , -0.10834 , -0.38753 ) ,
  D3DXVECTOR3(0.642738 , -0.0976995 , -0.759359 ) ,
  D3DXVECTOR3(-0.120142 , 0 , -0.7 ) ,
  D3DXVECTOR3(0.45 , -0.124391 , -0.95 ) ,
  D3DXVECTOR3(1.14463 , 0 , -0.772423 ) ,
  D3DXVECTOR3(1.08577 , 0 , -0.5 ) ,
  D3DXVECTOR3(0.595324 , -0.00733542 , -0.6 ) ,
  D3DXVECTOR3(0.948099 , -0.0595445 , -0.309415 ) ,
  D3DXVECTOR3(0.25 , 0 , 0.3 ) ,
  D3DXVECTOR3(0.865528 , 0 , 0.7 ) ,
  D3DXVECTOR3(0.15 , -0.0342669 , 0.05 ) ,
  D3DXVECTOR3(0.37072 , 0.111425 , -0.115931 ) ,
  D3DXVECTOR3(0.85798 , 0 , -0.55 ) ,
  D3DXVECTOR3(0.5 , 0.0933183 , 0.285312 ) ,
  D3DXVECTOR3(0.1 , -0.237763 , 0.45 ) ,
  D3DXVECTOR3(0.3 , 0 , 0.965509 ) ,
  D3DXVECTOR3(0.749898 , 0 , 0.240545 ) ,
  D3DXVECTOR3(1.22456 , 0 , 0.231804 ) ,
  D3DXVECTOR3(0.996245 , 0 , 0.000274636 ) ,
  D3DXVECTOR3(0.65 , 0.0786029 , 0 ) ,
  D3DXVECTOR3(0.35 , 0.146879 , -0.640148 ) ,
  D3DXVECTOR3(0.958345 , -0.00875026 , -0.743699 ) 
};

int StoredNumE[24] = {
2,1,1,
1,1,1,
2,1,1,
2,1,1,
2,2,1,
1,1,1,
1,1,0,
1,1,1
};

int StoredE[28] = {
1,12,2,
3,4,5,
6,7,23,
19,9,10,
22,11,15,
13,16,14,
15,7,21,
17,18,20,
18,20,20,
8
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph lp2 = { 
  24,
  28,
  &StoredDirGraphlp2::StoredV[0],
  &StoredDirGraphlp2::StoredNumE[0],
  &StoredDirGraphlp2::StoredE[0] 
};

////////////////////////////////////////////////////////////
//   lp3
////////////////////////////////////////////////////////////

namespace StoredDirGraphlp3 {

D3DXVECTOR3 StoredV[23] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.2 , 0 , 0.2 ) ,
  D3DXVECTOR3(0.15 , 0.0679963 , 0.7 ) ,
  D3DXVECTOR3(0.35 , 0 , 1.25 ) ,
  D3DXVECTOR3(0.65 , 0.128079 , 0.646551 ) ,
  D3DXVECTOR3(0.2 , -0.126635 , -0.45 ) ,
  D3DXVECTOR3(0.60056 , 0.15253 , -0.119979 ) ,
  D3DXVECTOR3(0.4 , 0 , 0.6 ) ,
  D3DXVECTOR3(0.7 , 0 , 1.15 ) ,
  D3DXVECTOR3(0.8 , 0 , 0.7 ) ,
  D3DXVECTOR3(0.350533 , -0.150588 , -0.0355817 ) ,
  D3DXVECTOR3(0.3 , 0 , -0.9 ) ,
  D3DXVECTOR3(0.7 , 0 , -1.38283 ) ,
  D3DXVECTOR3(1.13177 , 0 , -0.713627 ) ,
  D3DXVECTOR3(0.5 , 0 , -0.4 ) ,
  D3DXVECTOR3(0.7 , 0 , -0.3 ) ,
  D3DXVECTOR3(0.500829 , 0.130554 , -0.693443 ) ,
  D3DXVECTOR3(0.775909 , -0.10587 , -1.12209 ) ,
  D3DXVECTOR3(0.811752 , 0 , -0.15 ) ,
  D3DXVECTOR3(0.65 , -0.0671697 , 0.15 ) ,
  D3DXVECTOR3(0.572221 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(1.36972 , 0 , -0.5 ) 
};

int StoredNumE[23] = {
2,1,1,
1,1,2,
1,1,1,
1,1,2,
1,1,1,
1,1,1,
1,1,1,
0,1
};

int StoredE[25] = {
1,5,2,
3,4,19,
6,11,7,
8,9,10,
19,12,22,
16,14,15,
18,17,13,
20,20,21,
18
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph lp3 = { 
  23,
  25,
  &StoredDirGraphlp3::StoredV[0],
  &StoredDirGraphlp3::StoredNumE[0],
  &StoredDirGraphlp3::StoredE[0] 
};
////////////////////////////////////////////////////////////
//   twoLevel2
////////////////////////////////////////////////////////////

namespace StoredDirGraphtwoLevel2 {

D3DXVECTOR3 StoredV[14] = {
  D3DXVECTOR3(0 , 0 , 0 ) ,
  D3DXVECTOR3(0.123623 , 0 , -0.132702 ) ,
  D3DXVECTOR3(0.781189 , -0.0554 , 0 ) ,
  D3DXVECTOR3(0.836773 , 0 , -0.108317 ) ,
  D3DXVECTOR3(0.925537 , 0 , 0 ) ,
  D3DXVECTOR3(1 , 0 , 0 ) ,
  D3DXVECTOR3(0.05 , 0 , 0 ) ,
  D3DXVECTOR3(0.230483 , -0.056482 , 0 ) ,
  D3DXVECTOR3(0.170139 , 0 , -0.000488162 ) ,
  D3DXVECTOR3(0.340662 , 0.0685149 , 0.00106275 ) ,
  D3DXVECTOR3(0.65 , 0.070765 , 0 ) ,
  D3DXVECTOR3(0.764072 , 0.00318108 , 0.0915822 ) ,
  D3DXVECTOR3(0.834814 , 0 , 0 ) ,
  D3DXVECTOR3(0.240713 , 0.0265718 , 0.15 ) 
};

int StoredNumE[14] = {
1,1,1,
1,1,0,
2,1,1,
1,1,1,
1,1
};

int StoredE[14] = {
6,7,3,
4,5,1,
8,2,13,
10,11,12,
4,9
};

}; // end of namespace StoredDirGraph

DirectedGraph::StoredDirGraph twoLevel2 = { 
  14,
  14,
  &StoredDirGraphtwoLevel2::StoredV[0],
  &StoredDirGraphtwoLevel2::StoredNumE[0],
  &StoredDirGraphtwoLevel2::StoredE[0] 
};




}//   end namespace StoredPieces