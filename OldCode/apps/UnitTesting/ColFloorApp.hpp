#include  "../BaseDX.hpp"

#include  "Global.hpp"
#include	"RenderModels/DxMeshModel.hpp"
#include  "RenderModels/SingleBox.hpp"
#include  "Physics/RigidBody.hpp"
#include  "GameBall/TrackPath.hpp"
#include  "Project_Dependant/StoredSplines.hpp"

#include  "ObjEditor/Mesh/Mesh.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

class ColFloorApp : public BaseDX
{
public:
	ColFloorApp(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS);
	~ColFloorApp();

	bool init();
	void onResize();
	void drawScene();

  void  initShip();

private:

  Mesh* loadMesh ( const char* aFileName );


  SingleBox     mBox;


  std::vector<Mesh*>       mMeshTest;
  std::vector<EditableMeshModel*> mMeshModel;
  ColourMapping   mColours;

  TrackPath*    mTestTrack;
  DxMeshModelBatch<VertPNT>* mTestTrackModel;

  MeshCol* mTestCol;

  D3DXVECTOR3   mLastNormalOfContact;
  D3DXVECTOR3   mLastVelocityNonZero;
  D3DXVECTOR3   mLastCrossProd;

  int mLastT;

private:
  void initColours();
void  debugEnd();
  void  debugStart();

};
