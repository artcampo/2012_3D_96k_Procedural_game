#include  "../BaseDX.hpp"

#include  "Global.hpp"
#include	"RenderModels/DxMeshModel.hpp"
#include  "RenderModels/SingleBox.hpp"
#include  "Physics/RigidBody.hpp"


#include  "ObjEditor/Mesh/Mesh.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

class RigidApp : public BaseDX
{
public:
	RigidApp(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS);
	~RigidApp();

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

private:
  void initColours();

};
