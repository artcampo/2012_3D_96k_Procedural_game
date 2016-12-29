#ifndef _TrackOutPiecesJumps_HPP_
#define _TrackOutPiecesJumps_HPP_

#include  "dx_misc.hpp"
#include  "Geometry/DirGraph/DirGraph.hpp"
#include  "Geometry/DirGraph/Pattern.hpp"
#include  "Geometry/DirGraph/SubRule.hpp"

namespace StoredPiecesJumps{

  /////////////////////////////////////////////////////////////////////////
  //  Functions that generate the jump's graphs
  /////////////////////////////////////////////////////////////////////////
  DirectedGraph::PatternOut* patternOutRamp         ( int aId );
  DirectedGraph::PatternOut* patternOutNineTurn     ( int aId );
  DirectedGraph::PatternOut* patternOutOne80Turn    ( int aId );
  DirectedGraph::PatternOut* patternOutLoop         ( int aId );
  DirectedGraph::PatternOut* patternOutLateral      ( int aId );
  DirectedGraph::PatternOut* patternOutLateralCross ( int aId );
  DirectedGraph::PatternOut* patternOutTwoWayJump   ( int aId );
  DirectedGraph::PatternOut* patternOutThreeWayJump ( int aId );
  DirectedGraph::PatternOut* patternOutTwister      ( int aId );
  DirectedGraph::PatternOut* patternOutLevelCross   ( int aId );
  
  

  namespace General{
    const static int paddingLength    = 50;
  };

  /////////////////////////////////////////////////////////////////////////
  //  Data describing the various jumps
  /////////////////////////////////////////////////////////////////////////

  namespace Ramp{
    //  used to discrimante piece
    const static int subsMinDistance  = (120 + 2*General::paddingLength + 10 );
    //  used to build graph
    const static int jumpLength    = 30;
    const static int jumpHeight    = 5;
    const static int landLength    = 40;
    const static int landEnd       = 120;    
  };

  namespace NineTurn{
    //  used to discrimante piece
    const static int subsMinDistance  = 160;
    //  used to build graph / geometry 
    const static int jumpLength    = 30;
    const static int jumpHeight    = 5;
    const static int turnHeight    = 20;
    const static int turnBitanDist = 10;
    const static int landLength    = 80;
    const static int outEndB       = 40;   
    const static int outEndT       = (landLength + outEndB);    
     
  };

  namespace Loop{
    //  used to discrimante piece
    const static int subsMinDistance  = 140;
    //  used to build graph / geometry     
    const static int psLength      = 30;            
    const static int pbLength      = 40;
    const static int ptLength      = 50;
    const static int paLength      = 60;
    const static int peLength      = 70;            
    
    const static int pHeight       = 8;
    const static int ptHeight      = 16;
    
    const static int paWidthC      = 8;    
    const static int ptWidthC      = 16;
    const static int pbWidthC      = 24;    

    const static int outLength     = 90;
    const static int outWidthC     = 20; 
  };


  namespace TwoWayJump{
    //  used to discrimante piece
    //const static int subsMinDistance  = (140 + 4*General::paddingLength + 10 );
    const static int subsMinDistance  = 800;
    //  used to build graph / geometry     
    //const static int psLength      = 20;   
    const static int jumpLength    = 20;
    const static int b1Length      = 40;            
    const static int b2Length      = 30; 
    const static int peLength      = 120;  


    const static int jumpHeight    = 5;
    const static int b1Height      = 0;            
    const static int b2Height      = -40;      

    const static int e1Length      = 40; 
    const static int e2Length      = 40; 
    const static int e1Width       = -50;
  };

 namespace Twister{
    //  used to discrimante piece
    const static int subsMinDistance  = 100;
    const static int subsMaxDistance  = 200; 
  };

 namespace LevelCross{
    //  used to discrimante piece
    const static int subsMinDistance  = 1000;
    const static int subsMaxDistance  = 2000; 
    const static int lowLevelHeight   = -100; 
  };

/*
  namespace {
    //  used to discrimante piece
    //  used to build graph
    //  used to construct geometry
  };
  */  


};

#endif  //_TrackOutPiecesJumps_HPP_
