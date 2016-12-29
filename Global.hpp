#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_



#include <d3dx10.h>


#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"
#include  "Camera/Camera.hpp"

#include    "dx_misc.hpp"

//debug
#include <iostream>
#include <fstream>
#include <vector>


namespace Global{

extern ID3D10Device*    md3dDevice;


extern D3DXMATRIX       mWorld;
extern D3DXMATRIX       mView;
extern D3DXMATRIX       mProjection;

extern D3DXVECTOR3      mEyePos;
extern D3DXVECTOR3      camDir;


extern D3DXVECTOR3      mBallPosition;
extern D3DXVECTOR3      mBallVel;
extern float            mGameLapsedTime;
extern int              mPunctuation;


extern std::vector<std::ofstream*> mFileDebugBody;
extern std::ofstream*   mFileDebugTracks;
extern std::ofstream*   mFileDebugTrack;
extern std::ofstream*   mFileDebugContacts;
extern std::ofstream*   mFileDebugCol;
extern std::ofstream*   mFileDebugColOffenders;
extern std::ofstream*   mFileDebugTime;
extern std::ofstream*   mFileDebugTimeProf;
extern std::ofstream*   mFileDebugPhysGen;
extern std::ofstream*   mFileDebugColIt;
extern std::ofstream*   mFileDebugCamera;
extern std::ofstream*   mFileDebugKeys;
extern std::ofstream*   mFileDebugNumStatic;
extern std::ofstream*   mFileDebugFps;
extern std::ofstream*   mFileDebugPhysStep;


extern int mNitros;
extern int mLaps;

extern Camera*               gCam;
extern std::string gChronoDisplay;
extern std::string gIAvel;
extern std::string gIAdir;
extern int gIAedge;
extern std::string  gAImaxAngle;
extern std::string  gAIdistance;

extern int mShowIterTrack;
extern int mExcludeEdge;



};

namespace GlobalGame{

};
   
namespace GlobalDebugSSDO{
  extern float debug_singularity;
  extern float debug_sample_radius;
  extern float debug_depthBias;
  extern float debug_strength;
};

#endif 