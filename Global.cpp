#include		"Global.hpp"

//TODO: change prefixes
//TODO: change Global -> GlobalRender
namespace Global{
ID3D10Device*       md3dDevice;

D3DXMATRIX          mWorld;
D3DXMATRIX          mView;    
D3DXMATRIX          mProjection;

D3DXVECTOR3         mEyePos;    //Word Space
D3DXVECTOR3         camDir;     //Word Space
D3DXVECTOR3         mBallPosition;
D3DXVECTOR3         mBallVel;
float               mGameLapsedTime;
int                 mPunctuation;

Shader              gShaderTable[gNumStoredShaders];  
//gen
std::vector<std::ofstream*> mFileDebugBody;
std::ofstream* mFileDebugTracks;
std::ofstream* mFileDebugTrack;

//phys
std::ofstream* mFileDebugContacts;
std::ofstream* mFileDebugCol;
std::ofstream*   mFileDebugColOffenders;
std::ofstream* mFileDebugTime;
std::ofstream* mFileDebugTimeProf;
std::ofstream* mFileDebugColIt;
std::ofstream*   mFileDebugNumStatic;
std::ofstream*   mFileDebugPhysStep;
std::ofstream* mFileDebugPhysGen;


//game
std::ofstream*   mFileDebugCamera;
std::ofstream*   mFileDebugKeys;
std::ofstream*   mFileDebugFps;
int mNitros;

Camera*               gCam;
std::string gChronoDisplay;


int mShowIterTrack;
int mExcludeEdge = 0;
int mLaps;

std::string gIAvel;
std::string gIAdir;
int gIAedge;
std::string gAImaxAngle;
std::string  gAIdistance;

//SSDO
float debug_sample_radius;
float debug_singularity;
float debug_depthBias;

};

namespace GlobalDebugSSDO{
  float debug_singularity;
  float debug_sample_radius;
  float debug_depthBias;
  float debug_strength;
};

namespace GlobalGame{


//  Main ship
};