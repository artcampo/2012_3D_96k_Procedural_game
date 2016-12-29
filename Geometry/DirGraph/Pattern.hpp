#ifndef _Pattern_HPP
#define _Pattern_HPP

//#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>

#include "GameBall/TrackBp.hpp"
#include "Geometry/DirGraph/DirGraphDataTypes.hpp"
#include "Misc/StlHelpers.hpp"
#include "Geometry/DxMathHelpers.hpp"
#include "Geometry/DirGraph/DirGraph.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesGeneral.hpp"

namespace DirectedGraph {  

  class PatternInNode;
  class GeomNode;
  class Branch;
  class PatternOutNode;
  class PatternOut;
  class PatternIn;

  ///////////////////////////////////////////////////////////////
  //PatternInNode
  ///////////////////////////////////////////////////////////////
  class PatternInNode{

  public:
    int   degIn;
    int   degOut;
    std::vector<PatternInNode> mDesc;

  public:
    PatternInNode (){};
    PatternInNode ( const int aDegIn, const int aDegOut )
    {
      degIn   = aDegIn;
      degOut  = aDegOut;
    }
    PatternInNode ( const int aDegIn, const int aDegOut, 
                  const std::vector<PatternInNode>& aDesc )
    {
      degIn   = aDegIn;
      degOut  = aDegOut;
      mDesc   = aDesc;
    }
    PatternInNode ( const int aDegIn, const int aDegOut, 
                  const PatternInNode& aDesc )
    {
      degIn   = aDegIn;
      degOut  = aDegOut;
      mDesc.clear();
      mDesc.push_back( aDesc );
    }
  };

  ///////////////////////////////////////////////////////////////
  //GeomNode
  ///////////////////////////////////////////////////////////////
  class GeomNode{
  public:

    enum eAbsCoord{
      eAbsCoordEnd = true
    };
   enum eCoordPos{
      eCoordEnd = true
    };

    GeomNode ()
      {
      mIsTangentAbsorberBeg = false;
      mIsTangentAbsorberEnd = false;
      };   
    GeomNode( const float aTanCompLoc, 
              const float aBinCompLoc,
              const float aNorCompLoc,
              const float aTanCompAbs = 0.0f, 
              const float aBinCompAbs = 0.0f,
              const float aNorCompAbs = 0.0f,
              const bool  aIsEndingAbsCoord     = false,
              const bool  aUsesAbsCoord         = false,
              const bool  aForceUpwardsNormal   = false,             
              const bool  aIsCenterAbsCoord     = false
              )
    {
      mTanCompLoc  = aTanCompLoc;
      mBinCompLoc  = aBinCompLoc;
      mNorCompLoc  = aNorCompLoc;
      mTanCompAbs  = aTanCompAbs;
      mBinCompAbs  = aBinCompAbs;
      mNorCompAbs  = aNorCompAbs; 
      mIsEndingAbsCoord     = aIsEndingAbsCoord;
      mUsesAbsCoord         = aUsesAbsCoord;
      mForceUpwardsNormal   = aForceUpwardsNormal;      
      mIsTangentAbsorberBeg = false;
      mIsTangentAbsorberEnd = false;
      mIsCenterAbsCoord     = false;
    }   

    GeomNode static geomNodeAbsolute(  
              const float aTanCompAbs, 
              const float aBinCompAbs,
              const float aNorCompAbs )
    {
      GeomNode g  (0,0,0,
                   aTanCompAbs,aBinCompAbs,aNorCompAbs);      
      return g;
    }

    GeomNode static geomNodeCenterAbs(  
              const float aTanCompAbs, 
              const float aBinCompAbs,
              const float aNorCompAbs )
    {
      GeomNode g  (0,0,0,
                   aTanCompAbs,aBinCompAbs,aNorCompAbs);      
      g.mIsCenterAbsCoord = true;       
      return g;
    }

    GeomNode static geomNodeAbsorberBeg()
    {
      GeomNode g  (0,0,0,
                   0,0,0,
                   false,false,false);
      g.mIsTangentAbsorberBeg = true;
      return g;
    }

    GeomNode static geomNodeAbsorberEnd()
    {
      GeomNode g  (0,0,0,
                   0,0,0,
                   false,false,false);
      g.mIsTangentAbsorberEnd = true;
      return g;
    }

    void logWrite( std::ofstream* aStream )
    {
      *aStream << "Coord: " << mTanCompLoc <<","  << mBinCompLoc <<","  << mNorCompLoc <<"," 
                            << mTanCompAbs <<","  << mBinCompAbs <<","  << mNorCompAbs;

    }
  public:
    float mTanCompLoc;
    float mBinCompLoc;
    float mNorCompLoc;
    float mTanCompAbs;
    float mBinCompAbs;
    float mNorCompAbs;
    bool  mIsEndingAbsCoord;  
    bool  mUsesAbsCoord;
    bool  mForceUpwardsNormal; 
    bool  mIsTangentAbsorberBeg;
    bool  mIsTangentAbsorberEnd;
    bool  mIsCenterAbsCoord;
  };

  ///////////////////////////////////////////////////////////////
  //Branch
  ///////////////////////////////////////////////////////////////
  class Branch{
  public:

  public:    
    std::vector<int>  mEdgeUsed;      // ids of edges used by this branch 
    std::vector<int>  mEdgesToUpdate; // ids of edges that have to be updated    
  };

  ///////////////////////////////////////////////////////////////
  //PatternOutNode
  ///////////////////////////////////////////////////////////////
  class PatternOutNode{
  public:
    //  Static data, at load time
    GeomNode    mGeomRules;
    bool        mTangentAndNormal;
    D3DXVECTOR3 mTan;
    D3DXVECTOR3 mNor;
    bool        mIsMutable;
    int         mEdgeJumpId;
    std::vector<PatternOutNode*>    mDesc;
    bool        mTanNorUnmodified;
    bool        mJointNode;
    

    //  Dynamic data, for a current substitution
    bool      mVertexInGraphCreated;
    int       mVertexInGraphId;
    bool      mEdgesCreated;    
    std::vector<DirectedGraph::Edge>  mEdges;     // Edge, vertices corresponding to graph
                                                  // (not to this substitution!)    
    std::vector<int>                  mEdgesId;   // Edges id on branch
    //  temporal init data
  public:
    D3DXVECTOR3   mAccumulatedParentsTangent;
    D3DXVECTOR3   mAccumulatedDescendantsTangent;    
    int           mNumParents;
  public:
    D3DXVECTOR3 tangent() const;
    D3DXVECTOR3 normal() const;
    PatternOutNode (){};
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const bool aIsMutable = true,
                      const int aEdgeJumpId = TrackBp::eEdgeNormal,
                      const bool aTanNorUnmodified = true  )
    {
      mGeomRules                    = aGeomRules;   
      mTangentAndNormal             = false;
      mAccumulatedParentsTangent    = D3DXVECTOR3(0,0,0);      
      mAccumulatedDescendantsTangent= D3DXVECTOR3(0,0,0);
      mNumParents                   = 0;
      mIsMutable                    = aIsMutable;
      mEdgeJumpId = aEdgeJumpId;
      mTanNorUnmodified = aTanNorUnmodified;
    }
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const D3DXVECTOR3& aTan,
                      const D3DXVECTOR3& aNor,
                      const bool aIsMutable = true,
                      const int aEdgeJumpId = TrackBp::eEdgeNormal,
                      const bool aTanNorUnmodified = true )
    {
      mGeomRules        = aGeomRules;   
      mTangentAndNormal = true;
      mTan              = aTan;
      mNor              = aNor;
      mIsMutable        = aIsMutable;
      mEdgeJumpId = aEdgeJumpId;
      mTanNorUnmodified = aTanNorUnmodified;
    }

    PatternOutNode (  const GeomNode& aGeomRules, 
                      const std::vector<PatternOutNode*>& aDesc, 
                      const bool aIsMutable = true,
                      const int aEdgeJumpId = TrackBp::eEdgeNormal,
                      const bool aTanNorUnmodified = true )
    {
      mGeomRules                      = aGeomRules;      
      mDesc                           = aDesc;     
      mTangentAndNormal               = false;
      mAccumulatedParentsTangent      = D3DXVECTOR3(0,0,0);
      mAccumulatedDescendantsTangent  = D3DXVECTOR3(0,0,0);
      mNumParents                     = 0;
      mIsMutable                      = aIsMutable;
      mEdgeJumpId = aEdgeJumpId;
      mTanNorUnmodified = aTanNorUnmodified;
    }
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const D3DXVECTOR3& aTan,
                      const D3DXVECTOR3& aNor,
                      const std::vector<PatternOutNode*>& aDesc, 
                      const bool aIsMutable = true,
                      const int aEdgeJumpId = TrackBp::eEdgeNormal,
                      const bool aTanNorUnmodified = true )
    {
      mGeomRules  = aGeomRules;      
      mDesc       = aDesc;     
      mTangentAndNormal = true;
      mTan              = aTan;
      mNor              = aNor;
      mIsMutable        = aIsMutable;
      mEdgeJumpId = aEdgeJumpId;
      mTanNorUnmodified = aTanNorUnmodified;
    }

    PatternOutNode (  const GeomNode& aGeomRules, 
                      PatternOutNode* aDesc,
                      const bool aIsMutable = true,
                      const int aEdgeJumpId = TrackBp::eEdgeNormal,
                      const bool aTanNorUnmodified = true)
    {
      mGeomRules  = aGeomRules;      
      mDesc.clear();
      mDesc.push_back( aDesc );
      mTangentAndNormal = false;
      mAccumulatedParentsTangent  = D3DXVECTOR3(0,0,0);      
      mAccumulatedDescendantsTangent= D3DXVECTOR3(0,0,0);
      mNumParents                 = 0;
      mIsMutable        = aIsMutable;
      mEdgeJumpId = aEdgeJumpId;
      mTanNorUnmodified = aTanNorUnmodified;
    }
    //  ctor for an special piece
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const D3DXVECTOR3& aTan,
                      const D3DXVECTOR3& aNor,
                      PatternOutNode* aDesc,
                      const bool aIsMutable = true,
                      const int aEdgeJumpId = TrackBp::eEdgeNormal,
                      const bool aTanNorUnmodified = true)
    {
      mGeomRules  = aGeomRules;      
      mDesc.clear();
      mDesc.push_back( aDesc );
      mTangentAndNormal = true;
      mTan              = aTan;
      mNor              = aNor;
      mIsMutable        = aIsMutable;
      mEdgeJumpId       = aEdgeJumpId; 
      mTanNorUnmodified = aTanNorUnmodified;
    }

    TrackBp::Vertex pos ( const DxMath::TnbBasis& aBasisAB, 
                      const D3DXVECTOR3& aVectorAB,
                      const D3DXVECTOR3& aPosBeg, 
                      const D3DXVECTOR3& aPosEnd,
                      const D3DXVECTOR3& aTanBeg,
                      const D3DXVECTOR3& aTanEnd);
    private:      
      DxMath::TnbBasis limitTan( const D3DXVECTOR3& aVectorAB );      
  };

  ///////////////////////////////////////////////////////////////
  //PatternOut
  ///////////////////////////////////////////////////////////////
  
  class PatternOut{
  public:
    //  Azimut on degrees, converted automatically
    PatternOut( PatternOutNode* aRoot, 
                const int aOutPatternId,
                const float aMinLength      = 50.0f,
                const float aMaxLength      = 5000.0f,
                const float aMinAzimut      = StoredPiecesBalancers::General::subsMinAzimuth,
                const float aMaxAzimut      = StoredPiecesBalancers::General::subsMaxAzimuth,
                const float aMinPolarDelta  = 0.0f,
                const float aMaxPolarDelta  = StoredPiecesBalancers::General::subsMaxPolar,
                const bool  aIsTanAbsBeg    = false, 
                const bool  aIsTanAbsEnd    = false)
    {
      mRoot         = aRoot;
      mOutPatternId = aOutPatternId;
      mMinLength    = aMinLength;
      mMaxLength    = aMaxLength;
      mMinAzimut    = DxMath::degToRad( aMinAzimut );
      mMaxAzimut    = DxMath::degToRad( aMaxAzimut );
      mMinPolarDelta     = DxMath::degToRad( aMinPolarDelta );
      mMaxPolarDelta     = DxMath::degToRad( aMaxPolarDelta );
      mIsTanAbsBeg       = aIsTanAbsBeg;
      mIsTanAbsEnd       = aIsTanAbsEnd;
      createJointMark();
      createBranches();
      createTangents();
    }
    bool passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd,
                              const bool aDoNotTakeAzimutIntoAccount = false);
    bool validateInitialEdge( const DirectedGraph::DirGraph& aGraph, 
                              const int aStartingVertex,
                              const bool );
    bool  isTangentAbsorber()
    {
      return mIsTanAbsBeg || mIsTanAbsEnd;
    }

  public:
    //  Static data, at load time
    PatternOutNode*     mRoot;    
    int                 mOutPatternId;    
    std::vector<Branch> mBranches;  
    float               mMinLength;
    float               mMaxLength;
    float               mMinAzimut;
    float               mMaxAzimut;    
    float               mMinPolarDelta;
    float               mMaxPolarDelta;
    bool                mIsTanAbsBeg;
    bool                mIsTanAbsEnd;
    //  data computed before use
    int                 mNumParents;    
  public:
    
    //  Data used while substituting
    PatternOutNode*     mEndNode;
    std::vector<Edge*>  mEdges;     // pointers to this current subsitution's edges  
    float               mOriginalDesiredLength;

    // TODO: delete this
    //DirectedGraph::Edge mVertexAEdge;   //  fake starting vertex
    //DirectedGraph::Edge mVertexBEdge;   //  fake ending vertex
  private:
    //  compute branches of this pattern
    void createBranches();
    int  createBranchesRec  ( PatternOutNode* aVertex, std::vector<int>& aBranchEdges, int aBranchIndex );
    void createEdgeArraysRec( PatternOutNode* aVertex );
    void selectEdgesToUpdate();
    //  mark vertices near joint 
    void createJointMark();
    void createJointMarkREC( PatternOutNode* aVertex, PatternOutNode* aVertexParent );
    void createJointMarkClearParents( PatternOutNode* aVertex );
    void createJointMarkCountParents( PatternOutNode* aVertex );
    // compute tangents of internal vertices
    void createTangents();
    void createTangentsRec( PatternOutNode* aNode, 
                            const D3DXVECTOR3& aParentTan );    
  };

  void  initPatternOutEdges ( PatternOutNode* aPattern );

  

  ///////////////////////////////////////////////////////////////
  //PatternIn
  ///////////////////////////////////////////////////////////////

  class PatternIn{
  public:
    PatternIn( const PatternInNode& aMatch,
              const float aMinLength,
              const float aMaxLength,
              const int aInPatternId )
    {
      mMatch        = aMatch;
      mMinLength    = aMinLength;
      mMaxLength    = aMaxLength;
      mInPatternId = aInPatternId;
    }
    bool passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd );
  public:
    PatternInNode mMatch;
    float         mMinLength;
    float         mMaxLength;
    int           mInPatternId;
  };

};  // end namespace DirectedGraph


#endif // _Pattern_HPP
