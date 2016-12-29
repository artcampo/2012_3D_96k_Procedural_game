#ifndef _TrackOutPiecesBalancers_HPP_
#define _TrackOutPiecesBalancers_HPP_

#include  "dx_misc.hpp"
#include  "Geometry/DirGraph/DirGraph.hpp"
#include  "Geometry/DirGraph/Pattern.hpp"
#include  "Geometry/DirGraph/SubRule.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesGeneral.hpp"

namespace StoredPiecesBalancers{

  /////////////////////////////////////////////////////////////////////////
  //  Functions that generate the balancers's graphs
  /////////////////////////////////////////////////////////////////////////
  DirectedGraph::PatternOut* patternOutZigZagDown   ( int aId );
  DirectedGraph::PatternOut* patternOutZigZagUp     ( int aId );

  DirectedGraph::PatternOut* patternOutTanAbsorberBeg  ( int aId );
  DirectedGraph::PatternOut* patternOutTanAbsorberEnd  ( int aId );

/*
  namespace {
    //  used to discrimante piece
    //  used to build graph
    //  used to construct geometry
  };
  */  


};

#endif  //_TrackOutPiecesBalancers_HPP_
