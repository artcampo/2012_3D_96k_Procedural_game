#include  "../BaseDX.hpp"

#include  "Global.hpp"
#include  "GameBall/Game.hpp"
#include  "Misc/Log.hpp"

#include	"RenderModels/DxMeshModel.hpp"
#include  "ObjEditor/Mesh/Mesh.hpp"
#include  "Project_Dependant/IOMenu.hpp"
#include  "Project_Dependant/IOMainMenu.hpp"
#include  "Project_Dependant/IOLoadScreen.hpp"
#include  "Project_Dependant/IOinputBlocked.hpp"
#include  "GameBpMenu.hpp"


#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;
class IoInput;
class IoAbstract;
class GameBpMainMenu;

class GameBpApp : public BaseDX
{ 
public:
	GameBpApp(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS);
	~GameBpApp();

  //  Main Game
  int   Main();
	bool  init();
	void  onResize();
	void  drawScene(); 
  virtual void _storeFps( const int aFps  );

  int   menuMain();
  //  Load Screen
  void  initGameWithUserOptions();
  int   mainLoadScreen();
  void  drawTextLoad();  
  

private:
  bool          mDxInitialized;
  bool          mGameInitialized;
  bool          mGameInCourse;
  Game*         mGame;
  Log*          mLog;

  IOLoadScreen* mIOLoadScreen;

  //  User options
  int       mUserDifficulty;
  int       mUserBall;
  int       mUserTrack;
  int       mUserMode;
  
  //  time control
  int     mLastT;
  double  mLastTime;
  bool    mTimeInit;
  int     mLoadCompleteBar;

private:
  
  void  initGamePrevious();
  void  debugEnd();
  void  debugStart();

private:
  IOMenu        mIoMenu;
  IOMainMenu*   mIoMainMenu;
  int mIndexMenu;
  
  GameBpMenu*             mInGameMenu;
  GameBpMenu*             mGameOverMenu;
  GameBpMainMenu*         mMainMenu;

  void resetMenu();
  void displayInGameMenu() const;
  void displayGameOverMenu() const;  
  void drawMenuItem ( std::string& aString, 
    bool aHighlighted, 
    int aIndex, 
    bool aCentered = false,
    const bool aMenuWidget = false )  const;
  
  void drawText ( std::string& aString ) const;
  void drawText ( std::string& aString, bool aCentered, D3DXCOLOR aCol ) const;
  void drawText ( std::string& aString, const int aHeightDivider, const D3DXCOLOR aCol ) const;
  

  void inGameMenuRestore();
  void inGameMenuReset();
public:
  void inGameMenuShow();
private:
  
  enum eMenus{
    eMenuPlay,
    eMenuExit
  };


};
