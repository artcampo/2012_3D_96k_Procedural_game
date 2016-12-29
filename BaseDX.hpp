#ifndef _BASEDX_HPP_
#define _BASEDX_HPP_

#include <iostream>

#include	"dx_misc.hpp"

//#include	"ParticleManager.hpp"

#include    "Global.hpp"
#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"
#include    "Project_Dependant/StoredShaders.hpp"
#include    "Noise/RandomNumberGenerator.hpp"
#include    "RenderModels/Basics/SphereModel.hpp"
#include    "RenderModels/Basics/SingleBoxP.hpp"
#include    "Project_Dependant/IOAbstract.hpp"
#include	"Shader/ShaderManager.hpp"

class DxRenderTarget{
public:
  DxRenderTarget() : target(NULL), texture(NULL), resource(NULL) {};

  ID3D10RenderTargetView*   target;
  ID3D10Texture2D*          texture;
  ID3D10ShaderResourceView* resource;

  void release()
  {
    ReleaseCOM( target );
    ReleaseCOM( texture );
    ReleaseCOM( resource );
  }
  ;
};

class BaseDX
{
  public:

    BaseDX(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS);
    virtual ~BaseDX();
    //Windows
    virtual int Main() = 0;    
    //Modifiers
    void disableFps	();    
		void enableFps	();    
    //Time
    void startTimer();
    void calculateTime();
    float getDeltaTime() const;
    float mGetDeltaTime() const;
    float getExtendedTime();
    double getSeconds();
    float getCentiseconds();
    // Inheritance
    virtual bool init();
    virtual void onResize();
    virtual void getKeyboard( IoAbstract* aIOinput );
    virtual void getKeyboardMenu( IoAbstract* aIOinput );
    virtual void getKeyboardTextIn( IoAbstract* aIOinput );
    virtual void drawScene();
    void drawText();
    virtual LRESULT getMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    virtual void _storeFps(  const int aFps  ) = 0;

  protected:
	  //		Dx device
    ID3D10Device*           md3dDevice;
    IDXGISwapChain*         mSwapChain;
    ID3D10Texture2D*        mDepthStencilBuffer;
    ID3D10RenderTargetView* mRenderTargetView;
    ID3D10DepthStencilView* mDepthStencilView;
    //  Shaders
    ShaderManager           mShaderManager;
    //  Transformation matrices
    int                     mCurrentWidth, mCurrentHeight;    
    float                   mFov;
    float                   mNearPlane;
    float                   mFarPlane;
	  D3DXMATRIX              mProjection;
    D3DXMATRIX              mWorld;
    D3DXMATRIX              mView;   
	  //		Windows
    HINSTANCE hInstance;
    HWND screen;
    
    bool isFullScreen_;
    HICON appIcon;
    bool  keys[256];
    bool  keyReleased;
    std::wstring wTitle_;    
    D3DXCOLOR               mClearColor;      //Background color   
    //Mouse points
    D3DXVECTOR3 mMouseIncrement;        //Per-frame increment TODO: various increments between two render frmames!!
    D3DXVECTOR3 ptsBegin, ptsEnd, ptsBeginNoButtons;
    //Text
    bool		  showFPS_;
    ID3DX10Font* mFont;
    ID3DX10Font* mFontBig;
    std::wstring text;
    //Time
  	double secondsPerCount;  
	  double deltaTime;
    __int64 baseTime;
	  __int64 prevTime;
    double secondsCounter;
    //FPS
    unsigned int mLastFramePerSecondCount;
    unsigned int nFrame; 
    int   mKeybWaitCount;

    // *** Functions *** 

    bool initWindowsApp();
    bool initDirectx();
    bool initGameObjects(); 
public:
    void keybCleanAll();
public:
    PointLight*                         pLights;
    int                                 nLights;
    const static int                    maxDefLights = 4000;
    SphereModel*                        mSphere;
protected:
    void setDeferred();    
    void applyLightsDeferred();
    void compositeDeferred();

private:
    D3DXVECTOR3 vectorOverSemiSphere( void );
    void  setUpDeferred();
    void  setUpSSDO();
    void  setUpSSAO();
    void  drawLights	( PointLight* pLights, int* nLights );

    //  other deferred data
    ID3D10Buffer*						            screenQuadVB;
    //DxAnimModel*                        sphere;
    
    Rand::RandGen                       mRand;
    const static int                    mSsdoTexturePatternSize = 32;
    const static int                    mSsaoTexturePatternSize = 64;
    ID3D10Texture1D*                    mSemiSphereTexture;    
    ID3D10ShaderResourceView*           mSeedTextureSRV;
    ID3D10Texture1D*                    mSsaoTexture;    
    ID3D10ShaderResourceView*           mSsaoTextureSRV;
    ID3D10Texture2D*                    mPermTexture;
protected:
    ID3D10ShaderResourceView*           mPermTextureSRV;
private:

    DxRenderTarget      mGBuffCol;
    DxRenderTarget      mGBuffPos;
    DxRenderTarget      mGBuffNor;
    DxRenderTarget      mGBuffTan;
    DxRenderTarget      mGBuffLig;
    DxRenderTarget      mGBuffOcc;
    DxRenderTarget      mGBuffOccBlur;
    DxRenderTarget      mGBuffLightP;

private:
    bool createRenderTarget ( DxRenderTarget& aRenderTarget,
                              int             aFormat,
                              int             aWidth,
                              int             aHeight);

    void compositeDeferredSSDO();
    void compositeDeferredSSAO();
    void compositeDeferredBlur();
    void compositeDeferredFinal();

    void generatePermutationTexture();

private:
  void unitTests();
};

namespace WinIO{

std::wstring s2ws(const std::string& s);
};

#endif //_BASEDX_HPP_