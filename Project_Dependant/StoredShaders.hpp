#ifndef _STOREDSHADERS_HPP_
#define _STOREDSHADERS_HPP_

/*!
 * \file SotredShaders.hpp
 * \brief Static data with shaders definition
 * \author Arturo
 */

#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"


//===========================================================================
/*
  Shader table.
*/
//===========================================================================


extern ShaderClassData*  gStoredShaders[gNumStoredShaders];

enum eIndexShaders{
  indexShadDefComposition,
  indexShadDefSSDO,  
  indexShadDefSSAO, 
  indexShadDefLight,
  indexShadPostBlur,
  indexShadDefGeom,
  indexShadDefGeomV,
  indexShadDefGeomV_PNTUV_track,         
  indexShadDefGeomV_PC,           //lines with colours - debug
  indexShadDefGeomV_PNT_BallMetal,
  indexShadDefGeomV_PNT_BallGlass,
  indexShadDefGeomV_PNT_BallMeteor,
  indexShadDefGeomV_PNT_BallBouncy,
  indexShadDefGeomV_PNT_BallDemo,
  indexShadDefGeomV_PNT_MetCube,
  indexShadDefGeomV_P,
  indexShadDefGeomV_PNT_Arch,
  indexShadDefGeomV_PNT_Bumper,
  indexShadDefGeomV_PNT_BallChase,
  indexShadDefGeomV_PNT_BallSpartacus
};


#endif