#include		<d3dx10.h>
#include		<dxerr.h>

#include    "Global.hpp"
#include		"RenderModels/dxDef_animModel.hpp"

#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags


bool DxAnimModel::loadFromScene (aiNode * node, const struct aiMesh** mMeshes)
{
  _root = new DxAnimModelNode;

  loadFromSceneRec( node, mMeshes , _root );

  return true;
}


DxAnimModelNode* DxAnimModel::findNode ( string idname )
{
  DxAnimModelNode* ret = findNodeRec( _root, idname );
  return ret;
}
  

DxAnimModelNode* DxAnimModel::findNodeRec ( DxAnimModelNode* n, string idname )
{
  if ( n->_identifier == idname )
  {
    return n;
  }
  else
  {    
    if (n->_numChildren == 0)
      return NULL;
    else
    {
      for (int i = 0; i < n->_numChildren; ++i)
      {
        DxAnimModelNode* ret = findNodeRec ( n->_children[i], idname);
        if ( ret != NULL)
          return ret;
      }
      return NULL;
    }
  }
}

/****************************************************************************
    \param node node from assimp's scene to load
    \param storeNode, node where to put it (memory must be already allocated)
    \return true<=>everything ok
 ****************************************************************************/
bool DxAnimModel::loadFromSceneRec ( aiNode* node, const struct aiMesh** mMeshes, DxAnimModelNode* storeNode )
{

  //  Load fixed data
  D3DXMATRIX mat( (float *)(& node->mTransformation) );

  //mat._44 = 1.0;
  //D3DXMatrixInverse( &mat, NULL, &mat );
  /*
  mat._14 *= 0.01f;
  mat._24 *= 0.01f;
  mat._34 *= 0.01f;
  */
  mat._14 *= 1.0f;
  mat._24 *= 1.0f;
  mat._34 *= 1.0f;
  D3DXMatrixTranspose(&mat,&mat );
  storeNode->_transform = mat;
  //D3DXMatrixIdentity( &storeNode->_transform );

  //_identifier = string( (char *)&node->mName.data );
  storeNode->_identifier = string( node->mName.data );

 
  if ( node->mNumMeshes > 0 )
  {
    storeNode->_hasMesh = true;

    storeNode->_meshTriangles = new DxMeshModelBatch<VertexPos>(  sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    for (unsigned int n = 0; n < node->mNumMeshes; ++n)
    {
      const struct aiMesh* mesh = *(mMeshes + node->mMeshes[n] );

      //////////////////////////////////////////////////////////////////
      //  Construct triangulated mesh, could be a mesh made of 
      //    a) triangles
      //    b) quads
      
      unsigned int numIndices = mesh->mFaces->mNumIndices;
      unsigned int numOutputVertices;
      VertexPos *vert;

      if ( numIndices == 3 )
      {
        //  Case a) triangle
        numOutputVertices = mesh->mNumFaces * 3;
        vert = new VertexPos[ numOutputVertices ];
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
          for (unsigned int j = 0; j < 3; ++j)
          {
            unsigned int vertex_index = mesh->mFaces[i].mIndices[j];
            (vert + i*numIndices + j)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[vertex_index]);
          }
        }
        
      }
      else
      {
        //  Case b) quad
        numOutputVertices = mesh->mNumFaces * 3 * 2;
        vert = new VertexPos[ numOutputVertices ];
        VertexPos *v;
        v = vert;
        for ( unsigned int i = 0; i < mesh->mNumFaces; ++i, v+=6 )
        {

            unsigned int v0 = mesh->mFaces[i].mIndices[0];
            unsigned int v1 = mesh->mFaces[i].mIndices[1];
            unsigned int v2 = mesh->mFaces[i].mIndices[2];
            unsigned int v3 = mesh->mFaces[i].mIndices[3];

            (v + 0)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[v0]);
            (v + 1)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[v1]);
            (v + 2)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[v2]);

            (v + 3)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[v2]);            
            (v + 4)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[v3]);            
            (v + 5)->pos = *((D3DXVECTOR3 *)&mesh->mVertices[v0]);
        }        
      }
     
      DxMeshModel<VertexPos> m(  sizeof(VertexPos) , 
                      numOutputVertices,
                      vert );
                    
      storeNode->_meshTriangles->addMeshModel ( &m );
      delete [] vert;
    }
    //  Finish meshes of this node
    storeNode->_meshTriangles->createVB();
  }
  else
  { 
    //No mesh present
    storeNode->_hasMesh = false;
  }


  //Now create children
  storeNode->_numChildren = node->mNumChildren;
  storeNode->_children = new DxAnimModelNode*[ node->mNumChildren ];
  for (unsigned int n = 0; n < node->mNumChildren; ++n)
  {
    storeNode->_children[n] = new DxAnimModelNode();
  }

  for (unsigned int n = 0; n < node->mNumChildren; ++n)
  {
    loadFromSceneRec( node->mChildren[n], mMeshes, storeNode->_children[n] );
  }

  return true;
}




void DxAnimModel::displaySansShader        ( double t )
{
  D3DXMATRIX identity;
  D3DXMatrixIdentity(&identity);

  displayRecSansShader ( t,
              _root,
              identity
              );


}

void DxAnimModel::displayRecSansShader ( double t, DxAnimModelNode* node, D3DXMATRIX trans )
{
  D3DXMATRIX actual;
  if ( !node->_hasAnimation)
    D3DXMatrixMultiply( &actual, &node->_transform, &trans );
  else
  {
    //    Interpolate rotation
    double time = fmod( t, node->_animation.mDuration );
    int i;
    for (i = 0; time > node->_animation.mRotationKeys[i + 1].mTime ; ++i) ;
    aiQuaternion q;
    double timeLapse = (time - node->_animation.mRotationKeys[i].mTime) / node->_animation.mTicksPerSecond;
    aiQuaternion::Interpolate( 
              q,
              (const aiQuaternion)node->_animation.mRotationKeys[i].mValue,
              (const aiQuaternion)node->_animation.mRotationKeys[i + 1].mValue,
              (float)timeLapse );

    D3DXQUATERNION dxq ( q.x, q.y, q.z, q.w );
    D3DXMATRIX rot;
    D3DXMatrixRotationQuaternion ( &rot, (const D3DXQUATERNION*) &dxq );
    

    //    Interpolate Translation
    int j;
    for (j = 0; time > node->_animation.mPositionKeys[j + 1].mTime ; ++j) ;
    D3DXVECTOR3 v;
    double timeLapseTrans = (time - node->_animation.mPositionKeys[j].mTime) / node->_animation.mTicksPerSecond;
    D3DXVec3Lerp( 
              &v,
              (const D3DXVECTOR3 *) &node->_animation.mPositionKeys[j].mValue,
              (const D3DXVECTOR3 *) &node->_animation.mPositionKeys[j + 1].mValue,
              (float)timeLapseTrans );
    
    D3DXMATRIX translation;
    D3DXMatrixTranslation( &translation, v.x, v.y, v.z );
    

    // Apply translation & rotation
    /*
    D3DXMatrixMultiply( &actual, &trans, &trans );
    D3DXMatrixMultiply( &actual, &rot, &actual );
    */
    D3DXMatrixMultiply( &actual, &translation, &trans );
    D3DXMatrixMultiply( &actual, &rot, &actual );
  }

  if (node->_hasMesh)
  {
    
    node->_meshTriangles->DisplaySansShader();

  }

  for (int n = 0; n < node->_numChildren; ++n)
  {
    displayRecSansShader( t, node->_children[n], actual );
  }

}


DxAnimModel::DxAnimModel()
{
  _hasBoundingCirc  = false;
  _normalScaling    = 0.3f;
}

