#ifndef			DX_DEF_ANIM_MODEL
#define			DX_DEF_ANIM_MODEL

#define			WIN32_LEAN_AND_MEAN
#define			WIN32_EXTRA_LEAN
#include		<windows.h>

#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags

#include		<vector>
#include		<queue>
#include    "dx_misc.hpp"
#include    "RenderModels/DxMeshModel.hpp"



using namespace std;

class DxAnimation {
public:
  double        mDuration;
  double        mTicksPerSecond;
  unsigned int  mNumRotationKeys;
  aiQuatKey*    mRotationKeys;
  unsigned int  mNumPositionKeys;
  aiVectorKey*  mPositionKeys;
};

class DxAnimModelNode{
    
public:
    
    string              _identifier;      //string name of node

    //Transofmratoin data
    D3DXMATRIX          _transform;
    bool                _hasAnimation;
    DxAnimation         _animation;

    // Mesh data
    bool                _hasMesh;
    DxMeshModelBatch<VertexPos>*   _meshTriangles; 
    
    //Hierarchy
    DxAnimModelNode**   _children;
    int                 _numChildren;

    /////////////
    // functions
    DxAnimModelNode(){ _hasAnimation = false; };
};


class DxAnimModel {

public:

    DxAnimModel ();
    bool loadFromScene    ( aiNode* node, const aiMesh** mMeshes );
    void displaySansShader( double t );  

    void computeBoundingCircumference( );
    float         getBoundingCircumference();
    D3DXVECTOR3   getCenterMass();
    float         getBoundingCircumferenceAtCenter();

private:

    DxAnimModelNode*    _root;
    bool                _hasBoundingCirc;
    float               _BoundCircRadius;
    D3DXVECTOR3         _centerMass;
    int                 _centerMassNumPoints;
    bool                _hasBoundingCircCentered;
    float               _BoundCircRadiusCentered;    
    float               _normalScaling;
        

    bool loadFromSceneRec ( aiNode* node, const aiMesh** mMeshes, DxAnimModelNode* storeNode );
    void computeBoundingCircumferenceRec ( DxAnimModelNode* node, D3DXMATRIX trans );
    void computeBoundingCircumferenceCenteredRec ( DxAnimModelNode* node, D3DXMATRIX trans );

    DxAnimModelNode* findNode     ( string idname );
    DxAnimModelNode* findNodeRec  ( DxAnimModelNode* n, string idname );

    void displayRecSansShader       ( double t, DxAnimModelNode* node, D3DXMATRIX trans );
    
};



#endif