#include  "Geometry/DirGraph/Pattern.hpp"
#include  "Geometry/DirGraph/SubRule.hpp"
#include  "Global.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesGeneral.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"

namespace DirectedGraph {

void  initPatternOutEdges ( DirectedGraph::PatternOutNode* aPattern )
{
  aPattern->mEdgesCreated = false;

  for ( int i = 0; i < aPattern->mDesc.size() ; ++i )
    initPatternOutEdges( aPattern->mDesc[i] );
}

bool PatternOut::passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd,
                              const bool aDoNotTakeAzimutIntoAccount)
{
  bool valid = true;
  valid &= (aLength > mMinLength);
  valid &= (aLength < mMaxLength);
  if ( !valid ) 
  { 
#ifdef GBP_LOG_TRACK_CREATION
    *Global::mFileDebugTrack <<" Length: " << aLength << " of [ " << mMinLength << "," << mMaxLength << "]  ";
#endif
    
    return valid;
  }

  if ( !aDoNotTakeAzimutIntoAccount )
  {
    float tanAzimut = DxMath::azimut( DxMath::unitVector( aVertEnd.pos, aVertBeg.pos )  );  
    valid &= (tanAzimut > mMinAzimut);
    valid &= (tanAzimut < mMaxAzimut);
    if ( !valid ) 
    {
  #ifdef GBP_LOG_TRACK_CREATION
      *Global::mFileDebugTrack <<" azimut: " << tanAzimut << " of [ " << mMinAzimut << "," << mMaxAzimut << "]  ";
  #endif   
    }
  }

  return valid;
}

/*
    Checks the polar ang<les of both nodes of the edge
  */
bool    PatternOut::validateInitialEdge( const DirectedGraph::DirGraph& aGraph, 
                      const int aStartingVertex,
                      const bool aDoNotTakeAzimutIntoAccount )
{
  bool valid;

  valid = aGraph.numDesc( aStartingVertex ) == 1;  
  if (!valid) *Global::mFileDebugTrack << " more than one edge";
  if (!valid) return valid;

  std::vector<int> v = aGraph.descendants( aStartingVertex );
  const int endingVertex = v[0];

  const TrackBp::Vertex vBeg = aGraph.vertex( aStartingVertex );
  const TrackBp::Vertex vEnd = aGraph.vertex( endingVertex );

  //  Check length and azimuth
  float distanceBegEnd = DirectedGraph::distance( aGraph, aStartingVertex, endingVertex );
  valid &= passesRequirements( distanceBegEnd, vBeg, vEnd, aDoNotTakeAzimutIntoAccount );
  if (!valid) 
  {
#ifdef GBP_LOG_TRACK_CREATION
    *Global::mFileDebugTrack << " length or azimuth";
#endif
    
  }
  if (!valid) return valid;

  //  Check polar coords of beg
  if ( !mIsTanAbsEnd )
  {
    /*
    float polarTangent = DxMath::polar( vBeg.tan ); 
    D3DXVECTOR3 ab = DxMath::unitVector( vEnd.pos, vBeg.pos);
    float polarAB = DxMath::polar( ab ); 
    */
    float absPolarDelta = DxMath::polarAngleDistance( DxMath::unitVector( vEnd.pos, vBeg.pos), vBeg.tan );
    valid &= (absPolarDelta >= mMinPolarDelta);
    valid &= (absPolarDelta <= mMaxPolarDelta);
    
    if ( !valid ) 
    {
#ifdef GBP_LOG_TRACK_CREATION
      *Global::mFileDebugTrack <<" polar begin: " << absPolarDelta << " of [ " << mMinPolarDelta << "," << mMaxPolarDelta << "]  ";
#endif
      return valid;
    }
  }
  
  //  Check polar coords of end
  if ( !mIsTanAbsBeg )
  {
    float absPolarDelta = DxMath::polarAngleDistance( DxMath::unitVector( vEnd.pos, vBeg.pos), vEnd.tan );
    valid &= (absPolarDelta >= mMinPolarDelta);
    valid &= (absPolarDelta <= mMaxPolarDelta);
    
    if ( !valid ) 
    {
#ifdef GBP_LOG_TRACK_CREATION
      *Global::mFileDebugTrack <<" polar end: " << absPolarDelta << " of [ " << mMinPolarDelta << "," << mMaxPolarDelta << "]  ";
#endif
      
      return valid;
    }
  }

  return valid;
}

bool PatternIn::passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd)
{
  bool valid = true;

  valid &= (aLength > mMinLength);
  valid &= (aLength < mMaxLength);
  
  return valid;
}

void PatternOut::createTangents()
{
  DxMath::TnbBasis m;
  m.tan = D3DXVECTOR3(1,0,0);
  m.nor = D3DXVECTOR3(0,1,0);
  m.bin = D3DXVECTOR3(0,0,1);
  
  D3DXVECTOR3 currentPos = mRoot->pos( m, 
                                  D3DXVECTOR3(1000,0,0), 
                                  D3DXVECTOR3(0,0,0), 
                                  D3DXVECTOR3(1000,0,0),
                                  m.tan,
                                  m.tan).pos;

  for ( int i = 0; i < mRoot->mDesc.size(); ++i )
    createTangentsRec(  mRoot->mDesc[i], currentPos );
}

void PatternOut::createTangentsRec( PatternOutNode* aNode, 
                                    const D3DXVECTOR3& aParentTan )
{
  if (aNode->mDesc.size() == 0 )
  {
    mEndNode = aNode;
    return;
  }

  DxMath::TnbBasis m;
  m.tan = D3DXVECTOR3(1,0,0);
  m.nor = D3DXVECTOR3(0,1,0);
  m.bin = D3DXVECTOR3(0,0,1);
  
  D3DXVECTOR3 currentPos = aNode->pos( m, 
                                  D3DXVECTOR3(1000,0,0), 
                                  D3DXVECTOR3(0,0,0), 
                                  D3DXVECTOR3(1000,0,0),
                                  m.tan,
                                  m.tan).pos;
  aNode->mAccumulatedParentsTangent += DxMath::unitVector(
                                        currentPos, 
                                        aParentTan );  
  aNode->mNumParents++;

  for ( int i = 0; i < aNode->mDesc.size(); ++i )
  {
    aNode->mAccumulatedDescendantsTangent += DxMath::unitVector(
          aNode->mDesc[i]->pos( m, 
            D3DXVECTOR3(1000,0,0), 
            D3DXVECTOR3(0,0,0), 
            D3DXVECTOR3(1000,0,0),
            m.tan,
            m.tan).pos,
          currentPos );
    createTangentsRec(  aNode->mDesc[i], 
                        currentPos );
  }
}

D3DXVECTOR3 PatternOutNode::tangent() const
{
  if( mTangentAndNormal )
    return mTan;
  else
  {
    D3DXVECTOR3 tan = (1.0f/float(mNumParents + mDesc.size()))*
      ( mAccumulatedParentsTangent + mAccumulatedDescendantsTangent );
    D3DXVec3Normalize( &tan, &tan );
    //DEBUG
    //tan = D3DXVECTOR3(1,0,0);
    //-DEBUG
    return tan;
  }
}

D3DXVECTOR3 PatternOutNode::normal() const
{
  return D3DXVECTOR3(0,1,0);
}

void PatternOut::createJointMark()
{
  createJointMarkClearParents( mRoot );
  createJointMarkCountParents( mRoot );
  createJointMarkREC( mRoot, NULL );
}


void PatternOut::createJointMarkREC( PatternOutNode* aVertex, PatternOutNode* aVertexParent )
{  
  if ( aVertex->mDesc.size() >= 2 || aVertex->mNumParents >= 2 )
  {
    aVertex->mJointNode = true;    
    for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
      aVertex->mDesc[i]->mJointNode = true;    
    aVertexParent->mJointNode = true;    
  }

  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
    createJointMarkREC( aVertex->mDesc[i], aVertex );
}

void PatternOut::createJointMarkClearParents( PatternOutNode* aVertex )
{
  aVertex->mNumParents = 0;
  aVertex->mJointNode = false;
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
    createJointMarkClearParents( aVertex->mDesc[i] );
}

void PatternOut::createJointMarkCountParents( PatternOutNode* aVertex )
{  
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
    ++aVertex->mDesc[i]->mNumParents;

  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
    createJointMarkCountParents( aVertex->mDesc[i] );
}



void PatternOut::createBranches()
{
  //mEdges.push_back( &mVertexAEdge );
  //mEdges.push_back( &mVertexBEdge );

  createEdgeArraysRec( mRoot );
  initPatternOutEdges( mRoot );

  std::vector<int> branch;
  createBranchesRec( mRoot, branch, 0 );

  selectEdgesToUpdate();
}

void PatternOut::selectEdgesToUpdate()
{
   
  if ( mBranches.size() == 1 )
  {
    // If it only has one branch all edges have to be balanced
    for ( int i = 0; i < mEdges.size() ; ++i )
    {
      if( mEdges[i]->edgeData.isModifiable() )
        mBranches[0].mEdgesToUpdate.push_back( i ); 
    }
  }
  else
  {
    // If it has many branches select the edges of this branch    
    for ( int i = 0; i < mEdges.size() ; ++i )
    {
      bool foundInAllBranches = true;
      for ( int j = 0; foundInAllBranches && j < mBranches.size() ; ++j )
        foundInAllBranches &= StlHelpers::isInVector( mBranches[j].mEdgeUsed, i );

      if ( !foundInAllBranches )
        for ( int j = 0; j < mBranches.size() ; ++j )
          if ( mEdges[i]->edgeData.isModifiable()  &&
               StlHelpers::isInVector( mBranches[j].mEdgeUsed, i ) )
            mBranches[j].mEdgesToUpdate.push_back( i );

    }
  }
  
}


void PatternOut::createEdgeArraysRec( PatternOutNode* aVertex )
{
  aVertex->mEdges.resize( aVertex->mDesc.size() );
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )
  {
    
    createEdgeArraysRec( aVertex->mDesc[i] );
  }

}


int PatternOut::createBranchesRec( PatternOutNode* aVertex, std::vector<int>& aBranchEdges, int aBranchIndex )
{  
  //  Base case

  if( aVertex->mDesc.size() == 0 )
  {
    mBranches.resize( aBranchIndex + 1 );
    /*
    Wtf were those two lines adding???? n'importe quoi!
    mBranches[aBranchIndex].mEdgeUsed.push_back( 0 );
    mBranches[aBranchIndex].mEdgeUsed.push_back( 1 );
    */
    for ( int i = 0; i < aBranchEdges.size() ; ++i )
      mBranches[aBranchIndex].mEdgeUsed.push_back( aBranchEdges[i] );
    return aBranchIndex;
  }
 
  int currentBranchIndex = aBranchIndex;

  if( aVertex->mDesc.size() == 1 )
  {
    int i = 0;
    int index;
    if ( ! aVertex->mEdgesCreated )
    {
      aVertex->mEdgesCreated = true;

      index = mEdges.size();
      aVertex->mEdges[0].edgeData.mEdgeJumpId = aVertex->mEdgeJumpId;
      mEdges.push_back( &aVertex->mEdges[0] );            
      aVertex->mEdgesId.push_back( index );
      
      index = aVertex->mEdgesId[0];
    }
    else
      index = aVertex->mEdgesId[i];

    aBranchEdges.push_back( index );

    if ( i == 0 )
    {      
      currentBranchIndex = createBranchesRec( aVertex->mDesc[0], aBranchEdges, currentBranchIndex );
    }
    return currentBranchIndex;
  }

  //  Make copies of current branch
  std::vector< std::vector<int> > branchEdges; 
  branchEdges.resize( aVertex->mDesc.size() );
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
    branchEdges[i] = aBranchEdges;
  
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )
  {
    int index;
    if ( ! aVertex->mEdgesCreated )
    {
      aVertex->mEdgesCreated = true;
      for ( int j = 0; j < aVertex->mDesc.size() ; ++j )
      {
        index = mEdges.size();
        aVertex->mEdges[j].edgeData.mEdgeJumpId = aVertex->mEdgeJumpId;
        mEdges.push_back( &aVertex->mEdges[j] );            
        aVertex->mEdgesId.push_back( index );
      }
      index = aVertex->mEdgesId[0];
    }
    else
      index = aVertex->mEdgesId[i];

    branchEdges[i].push_back( index );
    if ( i == 0)
      currentBranchIndex = createBranchesRec( aVertex->mDesc[0], branchEdges[i], currentBranchIndex );       
    else
      currentBranchIndex = createBranchesRec( aVertex->mDesc[i], branchEdges[i], currentBranchIndex + 1 );       
  }

  return currentBranchIndex;
}



DxMath::TnbBasis PatternOutNode::limitTan( const D3DXVECTOR3& aVectorAB )
{
  using namespace StoredPiecesBalancers;

  if ( !( DxMath::azimut( aVectorAB ) >= DxMath::degToRad( General::subsMinAzimuth )
        &&DxMath::azimut( aVectorAB ) <= DxMath::degToRad( General::subsMaxAzimuth ) ))
  {
    D3DXVECTOR3 tanBeg = aVectorAB;
    tanBeg.y *= 0.75f;
    tanBeg = DxMath::normalizeVector( tanBeg );
    return limitTan( tanBeg );
  }
  else
  {
    DxMath::TnbBasis basis;
    basis.tan = aVectorAB;
    basis.nor = DxMath::alignToTangents   ( basis.tan, D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0) );
    basis.bin = DxMath::thirdVectorBasis  ( basis.tan, basis.nor );
    //DxMath::clean( basis.bin, 0.0001f );
    //DxMath::clean( basis.nor, 0.0001f );
    DxMath::cleanZeroes( basis.bin );
    DxMath::cleanZeroes( basis.nor );
    return basis;
  }
}

TrackBp::Vertex PatternOutNode::pos ( 
                      const DxMath::TnbBasis& aBasisAB, 
                      const D3DXVECTOR3& aVectorAB,
                      const D3DXVECTOR3& aPosBeg, 
                      const D3DXVECTOR3& aPosEnd,
                      const D3DXVECTOR3& aTanBeg,
                      const D3DXVECTOR3& aTanEnd)
    {
      float length = D3DXVec3Length( &aVectorAB );
      TrackBp::Vertex v;
      
      float tanLength = length*mGeomRules.mTanCompLoc;
      float binLength = length*mGeomRules.mBinCompLoc;
      float norLength = length*mGeomRules.mNorCompLoc;

      float tanLengthAbs = mGeomRules.mTanCompAbs;
      float binLengthAbs = mGeomRules.mBinCompAbs;
      float norLengthAbs = mGeomRules.mNorCompAbs;

      if( mGeomRules.mIsTangentAbsorberBeg )
      {
        DxMath::TnbBasis basisAB = limitTan( DxMath::normalizeVector( aVectorAB )  );
        D3DXVECTOR3 d, newTan;
        d = DxMath::normalizeVector( ((basisAB.tan + aTanBeg ) * 0.5f) );
        //d = basisAB.tan;
        newTan = DxMath::normalizeVector( ((3*basisAB.tan + aTanBeg ) * 0.25f) );

        v.pos = aPosBeg + d * StoredPiecesBalancers::General::tanAbsoberDistance;
        v.tan = newTan;
        v.nor = DxMath::alignToTangents(  v.tan,
                                          D3DXVECTOR3(1,0,0),                                
                                          normal() );  
        return v;
      }

      if( mGeomRules.mIsTangentAbsorberEnd )
      {
        D3DXVECTOR3 d;
        DxMath::TnbBasis basisAB = limitTan( DxMath::normalizeVector( aVectorAB ) );

        d = DxMath::normalizeVector( ((basisAB.tan + aTanEnd ) * 0.5f) );
        v.tan = d;
        v.nor = DxMath::alignToTangents(  v.tan,
                                          D3DXVECTOR3(1,0,0),                                
                                          normal() );       
        v.pos = aPosEnd ;
        {
          D3DXVECTOR3 t = -basisAB.tan;
          DxMath::cleanZeroes( t );
          D3DXVECTOR3 d = DxMath::normalizeVector( 
                                      (
                                        t
                                        -
                                        aTanEnd
                                      ) * 0.5f );
          //d = t;
          DxMath::cleanZeroes( d );
          v.pos += d * StoredPiecesBalancers::General::tanAbsoberDistance;                         
        }
        return v;
      }
      
      if( !mGeomRules.mIsEndingAbsCoord )
        v.pos = aPosBeg;
      else
        v.pos = aPosEnd;
      if( mGeomRules.mIsCenterAbsCoord )
        v.pos = (aPosBeg + aPosEnd)* 0.5f;

      if (!mGeomRules.mUsesAbsCoord )
      {
        //  Uses both relative and absolute
        v.pos = v.pos 
              + tanLength*aBasisAB.tan
              + binLength*aBasisAB.bin
              + norLength* aBasisAB.nor
              + tanLengthAbs*aBasisAB.tan
              + binLengthAbs*aBasisAB.bin
              + norLengthAbs* aBasisAB.nor;     
      }
      else
      {
        //  Uses only absolute
        D3DXVECTOR3 i = aVectorAB; i.y = 0;
        D3DXVec3Normalize( &i, &i );
        D3DXVECTOR3 j = D3DXVECTOR3(0,1,0);
        D3DXVECTOR3 k = DxMath::thirdVectorBasis( i, j );
        v.pos += fabs(aVectorAB.x) * mGeomRules.mTanCompLoc * i;            
        v.pos += aVectorAB.y * mGeomRules.mBinCompLoc * j;
        v.pos += length * mGeomRules.mNorCompLoc * k;
      }

      v.tan = DxMath::alignToTangents(aBasisAB.tan, 
                                    D3DXVECTOR3(1,0,0),                                       
                                    tangent());           

      if( mGeomRules.mForceUpwardsNormal || mJointNode )
        v.tan.y = 0.0f;

      v.nor = DxMath::alignToTangents(  v.tan,
                                        D3DXVECTOR3(1,0,0),                                
                                        normal() );          

      return v;    
  }

};

