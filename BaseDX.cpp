#include "BaseDX.hpp"
#include "Project_Dependant/IOinput.hpp"
#include "Project_Dependant/ConditionalCompilation.hpp"
#include <sstream>

//#define _DP_UNIT_TESTS

#ifdef _DP_UNIT_TESTS
  #include "Geometry/Interpolation.hpp"
#endif

//Extern call
LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BaseDX* app = 0;

	switch( msg )
	{
		case WM_CREATE:{			
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (BaseDX*)cs->lpCreateParams;
			return 0;
		}
	}

	if( app )
		return app->getMessage(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
  }


//Creator
BaseDX::BaseDX(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS):
               screen               (0),
               md3dDevice           (0),
               mSwapChain           (0),
               mDepthStencilBuffer  (0),
               mRenderTargetView    (0),
               mDepthStencilView    (0),
               mFont                (0),
               mFontBig             (0),
               appIcon              (0),
               hInstance            (handler),
               ptsBegin             (0.0f,0.0f,0.0f),
               ptsEnd               (0.0f,0.0f,0.0f),
               ptsBeginNoButtons    (0.0f,0.0f,0.0f),
               mCurrentWidth        (width),
               mCurrentHeight       (height),
               isFullScreen_        (isFullScreen),
               mClearColor          (WHITE),
               secondsCounter       (0.0f),
               nFrame               (0),
               prevTime             (0),
               baseTime             (0),
               deltaTime            (0.0),
               secondsPerCount      (0.0),
               showFPS_             (showFPS),
               wTitle_              (wTitle),
               mLastFramePerSecondCount(0),
               mRand                (42)
{
  mKeybWaitCount = 0;
  for (UINT point_i = 0; point_i < 256; ++point_i) 
    keys[point_i] = false; 
  //Text
  text = L"";
  //Set secondsPerCount
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  secondsPerCount = 1.0 / (double)countsPerSec; 

  mNearPlane                  = 0.1f;
  mFarPlane                   = 6000.0f;

  mMouseIncrement             = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

  GlobalDebugSSDO::debug_depthBias = 1;
  GlobalDebugSSDO::debug_sample_radius = 3.2f;
  GlobalDebugSSDO::debug_singularity = 10;
  GlobalDebugSSDO::debug_strength = 2.5f;

#ifdef _DP_UNIT_TESTS
  unitTests();
#endif
}

//Destructor

BaseDX::~BaseDX()
{
  mSwapChain->SetFullscreenState(false, 0);

  if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(md3dDevice);
  ReleaseCOM(mFont); 
  ReleaseCOM(mFontBig);

  delete mSphere;

	DestroyIcon(appIcon);
}

bool BaseDX::init()
{
  if( !initWindowsApp() )  return false;
  if( !initDirectx() )     return false;
  if( !initGameObjects() ) return false;
  
  return true;
}

bool BaseDX::initGameObjects()
{  
  return true;
}

bool BaseDX::initWindowsApp()
{
  //Icon
  appIcon =  ExtractIcon(hInstance,L"img\\icon.bmp",0);
  //Window class
  WNDCLASSEX wc;    
  wc.cbSize         = sizeof(WNDCLASSEX);
  wc.style          = CS_HREDRAW | CS_VREDRAW; 
  wc.lpfnWndProc    = WndProc;
  wc.cbClsExtra     = 0; 
  wc.cbWndExtra     = 0; 
  wc.hInstance      = hInstance;
  wc.hIcon          = appIcon;
  wc.hCursor        = LoadCursor(NULL,  IDC_ARROW); 
  wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName   = 0;
  wc.lpszClassName  = L"IconWndClassEx"; 
  wc.hIconSm        = appIcon;

  //Registramos la instancia WNDCLASS en windows para que podamos crear ventanas basados en ella.
  if(!RegisterClassEx(&wc)){
    MessageBox(0, L"RegisterClass   FAILED", 0, 0);
    return false;
  }

  RECT R = { 0, 0, mCurrentWidth, mCurrentHeight};
  AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
  unsigned int width  = R.right - R.left;
	unsigned int height = R.bottom - R.top;

  screen = CreateWindow(
    L"IconWndClassEx",
    wTitle_.c_str(), 
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, //Top left (0,0)
    CW_USEDEFAULT, 
    width, 
    height,
    0,
    0,
    hInstance,
    this);
  if(screen == NULL){
   MessageBox(0, L"CreateWindow FAILED", 0, 0);
   return false;
  }
  
  ShowWindow(screen, SW_SHOW);
  UpdateWindow(screen);

  return true;
}

bool BaseDX::initDirectx()
{
  #if defined(DEBUG) | defined(_DEBUG)
	  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  #endif
    
	DXGI_SWAP_CHAIN_DESC swapchain;
	swapchain.BufferDesc.Width                    = mCurrentWidth;
	swapchain.BufferDesc.Height                   = mCurrentHeight;
	swapchain.BufferDesc.RefreshRate.Numerator    = 60;
	swapchain.BufferDesc.RefreshRate.Denominator  = 1;
	swapchain.BufferDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain.BufferDesc.ScanlineOrdering         = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchain.BufferDesc.Scaling                  = DXGI_MODE_SCALING_UNSPECIFIED;
	// No multisampling.
	swapchain.SampleDesc.Count   = 1;
	swapchain.SampleDesc.Quality = 0;

	swapchain.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain.BufferCount  = 1;
	swapchain.OutputWindow = screen;  
	swapchain.Windowed     = !isFullScreen_;
	swapchain.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	swapchain.Flags        = 0;

                    
  D3D10_DRIVER_TYPE md3dDriverType  = D3D10_DRIVER_TYPE_HARDWARE;	

	UINT createDeviceFlags = 0;
  #if defined(DEBUG) || defined(_DEBUG)  
      createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
  #endif 
  IDXGIFactory1 *pFactory;
  HR(CreateDXGIFactory1(__uuidof(IDXGIFactory),(void**)(&pFactory)));
  IDXGIAdapter1 *pAdapter;
  pFactory->EnumAdapters1(0,&pAdapter);

  D3D10CreateDevice(pAdapter,md3dDriverType,0,createDeviceFlags,D3D10_SDK_VERSION,&md3dDevice);
  pFactory->CreateSwapChain(md3dDevice,&swapchain,&mSwapChain);  
  ReleaseCOM(pAdapter);
  ReleaseCOM(pFactory);
  
  //Init font
  D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
  fontDesc.Width           = 0;
  fontDesc.Weight          = 0;
  fontDesc.MipLevels       = 1;
  fontDesc.Italic          = false;
  fontDesc.CharSet         = DEFAULT_CHARSET;
  fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
  fontDesc.Quality         = DEFAULT_QUALITY;
  fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
  wcscpy_s(fontDesc.FaceName, L"Verdana");
	D3DX10CreateFontIndirect(md3dDevice, &fontDesc, &mFont); 

  fontDesc.Height          = 36;
  D3DX10CreateFontIndirect(md3dDevice, &fontDesc, &mFontBig); 
  

  Global::md3dDevice = md3dDevice;


  mShaderManager.init( md3dDevice );

  generatePermutationTexture();
  setUpDeferred();
	onResize();

  return true;
}
// I/O

void BaseDX::drawScene	()
{
  Global::md3dDevice->ClearRenderTargetView(mRenderTargetView, mClearColor);
	Global::md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
  Global::md3dDevice->OMSetDepthStencilState(0, 0);
}

void BaseDX::drawText()
{    
  ++nFrame;

  if( ( getDeltaTime()) >= 1.0f )
  {         
    _storeFps( int(nFrame) );
	  // Reset
    mLastFramePerSecondCount = nFrame;
	  nFrame = 0;
	  secondsCounter  += 1.0f;
  }

  float centiseconds = getCentiseconds();

  std::wostringstream textStream;   
  textStream.setf( ios_base::fixed, ios_base::floatfield );
  textStream.precision(4);

  if (showFPS_)        
    textStream << L"FPS: " << mLastFramePerSecondCount << L"\n";

  textStream 
    << L"Punctuation: " << Global::mPunctuation << L"\n"
    //<< L"Centiseconds: "<< centiseconds << L"\n"
    << L"Seconds: "<< centiseconds/100.0f << L"\n"
    << L"SecGame: "<< Global::mGameLapsedTime/100.0f << L"\n"
    
    << L"Pos: ("<< Global::mBallPosition.x<<","<< Global::mBallPosition.y<<","<< Global::mBallPosition.z<<")\n"
    //<< L"Vel: ("<< Global::mBallVel.x<<","<< Global::mBallVel.y<<","<< Global::mBallVel.z<<")\n"
    << L"Vel: "<< D3DXVec3Length( &Global::mBallVel )<<"\n"
    << L"Nitros: ("<< Global::mNitros <<")\n"
    << L"Subs: ("<< Global::mShowIterTrack <<")\n"
    << L"ExEdge: ("<< Global::mExcludeEdge  <<")\n"
    << L"Timer: ("<<  WinIO::s2ws(Global::gChronoDisplay)  <<")\n"
    << L"Laps: ("<<  Global::mLaps  <<")\n"
/*
    << L"IA-vel: ("<<  WinIO::s2ws(Global::gIAvel)  <<")\n"
    << L"AI-dir: ("<<  WinIO::s2ws(Global::gIAdir)  <<")\n"
    << L"AI-edg: ("<<  Global::gIAedge  <<")\n"
    << L"AI-ang: "<<  WinIO::s2ws(Global::gAImaxAngle)  <<"\n"
    << L"AI-dis: "<<  WinIO::s2ws(Global::gAIdistance)  <<"\n"
*/
    << L"SSDO-depth: "<< GlobalDebugSSDO::debug_depthBias <<"\n"
    << L"SSDO-radiu: "<< GlobalDebugSSDO::debug_sample_radius <<"\n"
    << L"SSDO-singul: "<< GlobalDebugSSDO::debug_singularity <<"\n"
    << L"SSDO-streng: "<< GlobalDebugSSDO::debug_strength <<"\n";

    
    
  text = textStream.str();
  
  RECT Rect = {5, 5, 0, 0};
  //SACA UN WARNING A SABER PORQUE
  mFont->DrawText(0, text.c_str(), -1, &Rect, DT_NOCLIP, BLACK);	
  //mFont->DrawText(0, text.c_str(), -1, &Rect, DT_NOCLIP, BLACK);	
  
}

void BaseDX::onResize()
{
  mGBuffCol.release();
  mGBuffPos.release();
  mGBuffNor.release();
  mGBuffTan.release();
  mGBuffLig.release();
  mGBuffOcc.release();
  mGBuffOccBlur.release();
  mGBuffLightP.release();

	ReleaseCOM( mRenderTargetView );
	ReleaseCOM( mDepthStencilView );
	ReleaseCOM( mDepthStencilBuffer );
	// Resize the swap chain and recreate the render target view.
	mSwapChain->ResizeBuffers(1, mCurrentWidth, mCurrentHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D10Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	ReleaseCOM(backBuffer);
	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width              = mCurrentWidth;
	depthStencilDesc.Height             = mCurrentHeight;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage              = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0; 
	depthStencilDesc.MiscFlags          = 0;

	md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	// Bind the render target view and depth/stencil view to the pipeline.
	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	// Set the viewport transform.
	D3D10_VIEWPORT vp;

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = mCurrentWidth;
	vp.Height   = mCurrentHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	md3dDevice->RSSetViewports(1, &vp);  

  mFov = ( float )D3DX_PI * 0.5f;
  D3DXMatrixPerspectiveFovLH( &Global::mProjection, mFov, mCurrentWidth / (FLOAT)mCurrentHeight, mNearPlane, mFarPlane );
  
  //  Create deferred buffers
  //  Create G-buffers and RenderViews


  createRenderTarget( mGBuffCol,
                      DXGI_FORMAT_R32G32B32A32_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffPos,
                      DXGI_FORMAT_R32G32B32A32_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffNor,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffTan,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffLig,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );
/*
  createRenderTarget( mGBuffOcc,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth/4, mCurrentHeight/4 );

  createRenderTarget( mGBuffOccBlur,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth/2, mCurrentHeight/2 );
 */
  createRenderTarget( mGBuffOcc,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffOccBlur,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffLightP,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );
  
}

void BaseDX::getKeyboard( IoAbstract* aIOinput )
{  

  if(keys[VK_ESCAPE]) {
    aIOinput->Move(KESC);
  }
  if(keys[VK_BACK]) {
    aIOinput->Move(KBACK);
  }
  if(keys[VK_RETURN]) {
    aIOinput->Move(KINTRO);
  }
    
	const float cameraMovementMultiplier = 10.0f;
  if(keys[VK_UP]) {            
    Global::gCam->incrementEyePos( &D3DXVECTOR3(0,0,cameraMovementMultiplier) );
    aIOinput->Move(KUP);
  }
  if(keys[VK_DOWN]) {
    Global::gCam->incrementEyePos( &D3DXVECTOR3(0,0,-cameraMovementMultiplier) );
    aIOinput->Move(KDOWN);
  }
  if(keys[VK_RIGHT]) {
    Global::gCam->incrementEyePos( &D3DXVECTOR3(cameraMovementMultiplier,0,0) );
    aIOinput->Move(KRIGHT);
  }  
  if(keys[VK_LEFT]) {
    Global::gCam->incrementEyePos( &D3DXVECTOR3(-cameraMovementMultiplier,0,0) );
    aIOinput->Move(KLEFT);
  }  
  if(keys[VK_SHIFT]) {
    aIOinput->Move(KSHIFT);
  }
  if(keys[VK_SPACE]) {
    aIOinput->Move(KSPACE);
  }
  if(keys['G']) {
    aIOinput->Move(KG);
  }
  if(keys['Y']) {
    aIOinput->Move(KY);
  }
 if(keys['T']) {
    aIOinput->Move(KT);
  }
  if(keys['W']) {
    aIOinput->Move(KW);
  }
  if(keys['S']) {
    aIOinput->Move(KS);
  }
  if(keys['A']) {
    aIOinput->Move(KA);
  }
  if(keys['D']) {
    aIOinput->Move(KD);
  }
  if(keys['U']) {
    aIOinput->Move(KU);
  }
  if(keys['H']) {
    aIOinput->Move(KH);
  }
  if(keys['J']) {
    aIOinput->Move(KJ);
  }
  if(keys['K']) {
    aIOinput->Move(KK);
  }
  if(keys['I']) {
    aIOinput->Move(KI);
  }
  if(keys['O']) {
    aIOinput->Move(KO);
  }

  if(keys['M']) {
    aIOinput->Move(KM);
  }
  if(keys['N']) {
    aIOinput->Move(KN);
  }

  if ( mKeybWaitCount != 0 )
  {
    --mKeybWaitCount;
    return;
  }
  if(keys['Z'])
  {
    --Global::mShowIterTrack;    
    mKeybWaitCount = 20;
  }
  if(keys['X'])
  {
    ++Global::mShowIterTrack;
    mKeybWaitCount = 20;
  }

}

void BaseDX::getKeyboardMenu( IoAbstract* aIOinput )
{    
  if (keyReleased)
  {
    aIOinput->Move(K_NON_EXISTANT);
    return;
  }

  if(keys[VK_ESCAPE]) {
    aIOinput->Move(KESC);
  }
  if(keys[VK_BACK]) {
    aIOinput->Move(KBACK);
  }
  if(keys[VK_RETURN]) {
    aIOinput->Move(KINTRO);
  }
    

  if(keys[VK_UP]) {
    aIOinput->Move(KUP);
  }
  if(keys[VK_RIGHT]) {
    aIOinput->Move(KRIGHT);
  }  
  if(keys[VK_DOWN]) {
    aIOinput->Move(KDOWN);
  }
  if(keys[VK_LEFT]) {
    aIOinput->Move(KLEFT);
  }  
}

void BaseDX::getKeyboardTextIn( IoAbstract* aIOinput )
{    
  if (keyReleased)
  {
    aIOinput->Move(K_NON_EXISTANT);
    return;
  }

  for ( int i = 0; i < 256; ++i )
    if(keys[i]) 
    {
      aIOinput->Move(i);
      keys[i] = false;
    }
  
}

//Extern, it's called by the SO
LRESULT BaseDX::getMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{  
  switch(msg){
    case WM_LBUTTONDOWN: {  
      // Capture mouse input.  
        SetCapture(screen);  
      // Retrieve the screen coordinates of the client area, 
      // and convert them into client coordinates. 
      RECT rcClient;                  // client area rectangle     
      POINT ptClientUL;             // client upper left corner 
      POINT ptClientLR;              // client lower right corner
      GetClientRect(screen, &rcClient); 
      ptClientUL.x = rcClient.left; 
      ptClientUL.y = rcClient.top;  
      // Add one to the right and bottom sides, because the 
      // coordinates retrieved by GetClientRect do not 
      // include the far left and lowermost pixels.  
      ptClientLR.x = rcClient.right + 1; 
      ptClientLR.y = rcClient.bottom + 1; 
      ClientToScreen(screen, &ptClientUL); 
      ClientToScreen(screen, &ptClientLR);  
      // Copy the client coordinates of the client area 
      // to the rcClient structure. Confine the mouse cursor 
      // to the client area by passing the rcClient structure 
      // to the ClipCursor function. 
      SetRect(&rcClient, ptClientUL.x, ptClientUL.y, 
          ptClientLR.x, ptClientLR.y); 
      ClipCursor(&rcClient); 
      // Convert the cursor coordinates into a POINTS 
      // structure, which defines the beginning point of the 
      // line drawn during a WM_MOUSEMOVE message.  
      ptsBegin.x = (float)GET_X_LPARAM(lParam); 
      ptsBegin.y = (float)GET_Y_LPARAM(lParam);       
      
      return 0; 
    }
    case WM_MOUSEMOVE: 
      // When moving the mouse, the user must hold down 
      // the left mouse button 
      if (wParam & MK_LBUTTON) { 
          ptsEnd.x = (float)GET_X_LPARAM(lParam); 
          ptsEnd.y = (float)GET_Y_LPARAM(lParam); 
          mMouseIncrement = ptsEnd - ptsBegin;
          ptsBegin = ptsEnd;           
          D3DXVECTOR3 n = D3DXVECTOR3(1,0,0)*mMouseIncrement.x*0.5f +
                          D3DXVECTOR3(0,1,0)*mMouseIncrement.y*0.5f;
          Global::gCam->incrementEyePos( &n );

      } 
      if (wParam & MK_RBUTTON ) { 

          ptsEnd.x = (float)GET_X_LPARAM(lParam); 
          ptsEnd.y = (float)GET_Y_LPARAM(lParam); 
          mMouseIncrement = ptsEnd - ptsBegin;
          ptsBegin = ptsEnd;  
          if (!(wParam & MK_SHIFT) )
          {
            //D3DXVECTOR3 n = ptsBegin - D3DXVECTOR3(mCurrentWidth/2, mCurrentHeight/2, 0);
            D3DXVECTOR3 n = ptsEnd-ptsBeginNoButtons;
            n *= 0.01f;            
            n.y = 0;            
            Global::gCam->rotateAroundTarget( &n );
            //Global::gCam->incrementTargetPos( &n );
            
          }
          else
          {
            float n = (mMouseIncrement.x + mMouseIncrement.y) *0.01f;                                                  
            Global::gCam->incrementAtValue( n );
          }
      }     
      ptsBeginNoButtons.x = (float)GET_X_LPARAM(lParam);
      ptsBeginNoButtons.y = (float)GET_Y_LPARAM(lParam);
      return 0;

    case WM_LBUTTONUP: 
      ClipCursor(0); 
      ReleaseCapture(); 
      return 0;  

    case WM_KEYDOWN:     
      keys[wParam] = true;
      keyReleased = false;
     return 0;

    case WM_KEYUP:
      keys[wParam] = false; 
      keyReleased = true;
      return 0;    
  

    case WM_DESTROY:
      PostQuitMessage(0);

      return 0;

  	case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_SCREENSAVE:	
				case SC_MONITORPOWER:	

				return 0;	
			}
			break;	
    case WM_SIZE:       
	    //mCurrentWidth  = LOWORD(lParam);
	    //mCurrentHeight = HIWORD(lParam);
      if( md3dDevice ) {			  
			  if( wParam == SIZE_RESTORED ){  				
					  onResize();
				}
      }
      return 0;
  }  
  return DefWindowProc(screen, msg, wParam, lParam);
}

//Modifiers
void BaseDX::enableFps	()
{
	showFPS_ = true;
}

void BaseDX::disableFps()
{
	showFPS_ = false;
}

//Time
void BaseDX::startTimer()
{
	__int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  prevTime = baseTime = currTime;
  nFrame   = 0;
}
void BaseDX::calculateTime()
{
  __int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	
	deltaTime = (currTime - prevTime)*secondsPerCount;
	prevTime  = currTime;  
}

double BaseDX::getSeconds()
{   
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	

  return (currTime - baseTime) * secondsPerCount;
}


float BaseDX::getDeltaTime() const
{   
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	
  
  return ((currTime - baseTime) * (float)secondsPerCount) - (float)secondsCounter;
}

float BaseDX::getCentiseconds()
{
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	

  return (currTime - baseTime) * (float)secondsPerCount * 100.0f;
}



//===========================================================================
/*!
    Returns random vector over a semi sphere

    \return     D3DXVECTOR3 vector
*/
//===========================================================================
D3DXVECTOR3 BaseDX::vectorOverSemiSphere( void )
{  
  D3DXVECTOR3 N(0.0f, 1.0f,0.0f);
  float x,y,z;
  while (true) {

    x = mRand.randFloat_0_1()*2.0f - 1.0f; // random776 float between -1 and 1
    y = mRand.randFloat_0_1()*2.0f - 1.0f;
    z = mRand.randFloat_0_1()*2.0f - 1.0f;
    if (x * x + y * y + z * z > 1) continue; // ignore ones outside unit
                                             
     // sphere
    
    D3DXVECTOR3 v(x, y, z);
    //if (D3DXVec3Dot(&v, &N) < 0) continue; // ignore "down" dirs
    if (v.y < 0) continue; // ignore "down" dirs
    
    D3DXVECTOR3 d(x, y, z);
    //D3DXVec3Normalize( &d, &d);
    return d; // success!
  }
}

//===========================================================================
/*!
    Sets up deferred rendering

    \return     bool true <=> Shader compiled succesfully
*/
//===========================================================================
void BaseDX::setUpDeferred()
{
  pLights = new PointLight[maxDefLights];

  setUpSSDO();
  setUpSSAO();


	// Create full Screen Quad Vertices
	{
		ScreenVertex svQuad[4];
		svQuad[0].pos = D3DXVECTOR4( -1.0f, 1.0f, 0.5f, 1.0f );
		svQuad[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		svQuad[1].pos = D3DXVECTOR4( 1.0f, 1.0f, 0.5f, 1.0f );
		svQuad[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		svQuad[2].pos = D3DXVECTOR4( -1.0f, -1.0f, 0.5f, 1.0f );
		svQuad[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		svQuad[3].pos = D3DXVECTOR4( 1.0f, -1.0f, 0.5f, 1.0f );
		svQuad[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

		D3D10_BUFFER_DESC vbdesc =
		{
			4 * sizeof( ScreenVertex ),
			D3D10_USAGE_DEFAULT,
			D3D10_BIND_VERTEX_BUFFER,
			0,
			0
		};
		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = svQuad;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		md3dDevice->CreateBuffer( &vbdesc, &InitData, &screenQuadVB );
	}

  //  Load sphere for lighting
  mSphere = new SphereModel( 4 );
}

void BaseDX::setUpSSAO()
{
    //  Create Vectors over a semisphere
    D3DXVECTOR3 data[mSsaoTexturePatternSize * mSsaoTexturePatternSize];
    for (int i = 0; i < mSsaoTexturePatternSize * mSsaoTexturePatternSize; i++)  
    {
        float angle = mRand.randFloat_0_1() *2*DxMath::msPI;
        data[i].x = sin( angle );
        data[i].y = cos( angle );
        data[i].z = mRand.randFloat_0_1()*0.5 + 0.5;
    }         

    D3D10_TEXTURE1D_DESC desc;
    desc.Width = mSsaoTexturePatternSize*mSsaoTexturePatternSize;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;    
    desc.Usage = D3D10_USAGE_IMMUTABLE;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    
    D3D10_SUBRESOURCE_DATA dataDesc;
    dataDesc.pSysMem = data;    
    dataDesc.SysMemPitch = sizeof(float)*3;
    md3dDevice->CreateTexture1D(&desc, &dataDesc, &mSsaoTexture);
    
    D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

    ZeroMemory( &descSRV, sizeof(descSRV) );
    descSRV.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    descSRV.Texture2D.MipLevels = 1;
    descSRV.Texture2D.MostDetailedMip = 0;

    md3dDevice->CreateShaderResourceView( mSsaoTexture, &descSRV, &mSsaoTextureSRV ) ;
}

void BaseDX::setUpSSDO()
{
  //  Create Vectors over a semisphere
  D3DXVECTOR3 data[mSsdoTexturePatternSize * mSsdoTexturePatternSize];
  for (int i = 0; i < mSsdoTexturePatternSize * mSsdoTexturePatternSize; i++)  
      data[i] = vectorOverSemiSphere();
  

  D3D10_TEXTURE1D_DESC desc;
  desc.Width = mSsdoTexturePatternSize*mSsdoTexturePatternSize;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;    
  desc.Usage = D3D10_USAGE_IMMUTABLE;
  desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;
  
  D3D10_SUBRESOURCE_DATA dataDesc;
  dataDesc.pSysMem = data;
  //dataDesc.SysMemPitch = mSsdoTexturePatternSize*sizeof(float)*3;
  dataDesc.SysMemPitch = sizeof(float)*3;
  md3dDevice->CreateTexture1D(&desc, &dataDesc, &mSemiSphereTexture);
  
  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

  ZeroMemory( &descSRV, sizeof(descSRV) );
  descSRV.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
  descSRV.Texture2D.MipLevels = 1;
  descSRV.Texture2D.MostDetailedMip = 0;

  md3dDevice->CreateShaderResourceView( mSemiSphereTexture, &descSRV, &mSeedTextureSRV ) ;
}


//===========================================================================
/*!
    Deferred rendering: To be called prior to rendering
*/
//===========================================================================
void BaseDX::setDeferred()
{
  ID3D10RenderTargetView* mRenderTargetViewTable[5];  

  mRenderTargetViewTable[0] = mGBuffPos.target;
  mRenderTargetViewTable[1] = mGBuffCol.target;
  mRenderTargetViewTable[2] = mGBuffNor.target;
  mRenderTargetViewTable[3] = mGBuffTan.target;
  mRenderTargetViewTable[4] = mGBuffLightP.target;
  Global::md3dDevice->OMSetRenderTargets( 5, &mRenderTargetViewTable[0], mDepthStencilView);

  Global::md3dDevice->ClearRenderTargetView(mGBuffPos.target, BLACK);
  Global::md3dDevice->ClearRenderTargetView(mGBuffCol.target, WHITE);
  Global::md3dDevice->ClearRenderTargetView(mGBuffNor.target, BLACK);
  Global::md3dDevice->ClearRenderTargetView(mGBuffTan.target, BLACK);
  Global::md3dDevice->ClearRenderTargetView(mGBuffLightP.target, WHITE);
  
}

//===========================================================================
/*!
    Deferred rendering: Composite final image
*/
//===========================================================================
void BaseDX::compositeDeferred()
{	
  //compositeDeferredSSDO();
  //compositeDeferredSSAO();
  //compositeDeferredBlur();
  compositeDeferredFinal();
  /*
#ifndef ___DP_DISABLE_SSAO
  compositeDeferredSSAO();
  #ifndef ___DP_DEBUG_SSAO__
    compositeDeferredBlur();
  #endif
#endif

#ifndef ___DP_DEBUG_SSAO_
  compositeDeferredFinal();
#endif
  */
}


void BaseDX::compositeDeferredSSDO()
{
// Bind the render target view and depth/stencil view to the pipeline.
  md3dDevice->OMSetRenderTargets( 1, &mGBuffOcc.target, mDepthStencilView);
  //md3dDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView);

  //  Set input textures  
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textPos")), mGBuffPos.resource );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textNor")), mGBuffNor.resource );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textTan")), mGBuffTan.resource );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textCol")), mGBuffCol.resource );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textLig")), mGBuffLig.resource );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textSeed")), mSeedTextureSRV );


  //  Set debug variables
  mShaderManager.mShaderTable[indexShadDefSSDO].SetRawValue( &(std::string("debug_strength")), &GlobalDebugSSDO::debug_strength, 1);
  mShaderManager.mShaderTable[indexShadDefSSDO].SetRawValue( &(std::string("debug_singularity")), &GlobalDebugSSDO::debug_singularity, 1);
  mShaderManager.mShaderTable[indexShadDefSSDO].SetRawValue( &(std::string("debug_sample_radius")), &GlobalDebugSSDO::debug_sample_radius , 1);
  mShaderManager.mShaderTable[indexShadDefSSDO].SetRawValue( &(std::string("debug_depthBias")), &GlobalDebugSSDO::debug_depthBias, 1);


  //  Set matrices
  mShaderManager.mShaderTable[indexShadDefSSDO].SetMatrix( &(std::string("ModelView")), &Global::mView );
  D3DXMATRIX m = Global::mView*Global::mProjection;
  mShaderManager.mShaderTable[indexShadDefSSDO].SetMatrix( &(std::string("ModelViewProj")), &m );
  //mShaderManager.mShaderTable[indexShadDefSSDO].SetMatrix( &(std::string("ModelViewProj")), &Global::mProjection );

  /////////////////////////////////////////////////////////
  //    Compute SSDO

  //Draw quad covering screen
  UINT strides = sizeof( ScreenVertex );
	UINT offsets = 0;
	ID3D10Buffer* pBuffers[1] = { screenQuadVB };

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefSSDO].mVertexLayout );
  Global::md3dDevice->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
	Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	mShaderManager.mShaderTable[indexShadDefSSDO].mTech->GetPassByIndex( 0 )->Apply( 0 );
	// Composite the blurred model into the original image.
	Global::md3dDevice->Draw( 4, 0 );

  /////////////////////////
  //  Unbound resources
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textPos")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textNor")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textTan")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textCol")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textLig")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSDO].SetResource( &(std::string("textSeed")), 0 );

  mShaderManager.mShaderTable[indexShadDefSSDO].mTech->GetPassByIndex( 0 )->Apply( 0 );
  
}

void BaseDX::compositeDeferredSSAO()
{
// Bind the render target view and depth/stencil view to the pipeline.
#ifndef ___DP_DEBUG_SSAO__
  md3dDevice->OMSetRenderTargets( 1, &mGBuffOcc.target, mDepthStencilView);
#endif
#ifdef ___DP_DEBUG_SSAO__
  md3dDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView);
#endif

  //  Set input textures  
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textPos")), mGBuffPos.resource );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textNor")), mGBuffNor.resource );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textTan")), mGBuffTan.resource );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textCol")), mGBuffCol.resource );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textLig")), mGBuffLig.resource );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textRandDir")), mSsaoTextureSRV );


  //  Set debug variables
  float radius = 1;
  float res[2]; float invRes[2];
  res[0] = mCurrentWidth;
  res[1] = mCurrentHeight;  
  invRes[0] = 1.0f / float(mCurrentWidth);
  invRes[1] = 1.0f / float(mCurrentHeight);
  float tanBias = 1.4;
  float focal[2];float invFocal[2];
  focal[0] = 1.0f / tanf( mFov*0.5 )*mCurrentHeight / mCurrentWidth ;
  focal[1] = 1.0f / tanf( mFov*0.5 );
  invFocal[0] =  1.0f/focal[0];
  invFocal[1] =  1.0f/focal[1];
  
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_TanBias")), &tanBias, 1);  
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_Radius")), &radius, 1);  
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_Resolution")), res, 2);
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_InvResolution")), invRes , 2);
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_Focal")), focal , 2);
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_InvFocal")), invFocal , 2);
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_NearPlane")), &mNearPlane , 1);
  mShaderManager.mShaderTable[indexShadDefSSAO].SetRawValue( &(std::string("g_FarPlane")), &mFarPlane , 1);
  

  //  Set matrices
  mShaderManager.mShaderTable[indexShadDefSSAO].SetMatrix( &(std::string("View")), &Global::mView );
  D3DXMATRIX m = Global::mView*Global::mProjection;
  mShaderManager.mShaderTable[indexShadDefSSAO].SetMatrix( &(std::string("ViewProj")), &m );
  //mShaderManager.mShaderTable[indexShadDefSSAO].SetMatrix( &(std::string("ModelViewProj")), &Global::mProjection );

  /////////////////////////////////////////////////////////
  //    Compute SSDO

  //Draw quad covering screen
  UINT strides = sizeof( ScreenVertex );
	UINT offsets = 0;
	ID3D10Buffer* pBuffers[1] = { screenQuadVB };

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefSSAO].mVertexLayout );
  Global::md3dDevice->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
	Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	mShaderManager.mShaderTable[indexShadDefSSAO].mTech->GetPassByIndex( 0 )->Apply( 0 );
	// Composite the blurred model into the original image.
	Global::md3dDevice->Draw( 4, 0 );

  /////////////////////////
  //  Unbound resources
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textPos")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textNor")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textTan")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textCol")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textLig")), 0 );
  mShaderManager.mShaderTable[indexShadDefSSAO].SetResource( &(std::string("textRandDir")), 0 );

  mShaderManager.mShaderTable[indexShadDefSSAO].mTech->GetPassByIndex( 0 )->Apply( 0 );
  
}


void BaseDX::compositeDeferredBlur()
{

  mShaderManager.mShaderTable[indexShadPostBlur].SetResource( &(std::string("textCol")), mGBuffOcc.resource );

  float width   = float(mCurrentWidth);
  float height  = float(mCurrentHeight);
  mShaderManager.mShaderTable[indexShadPostBlur].SetRawValue( 
      &(std::string("Width")), 
      &width,
      1);
  mShaderManager.mShaderTable[indexShadPostBlur].SetRawValue( 
      &(std::string("Height")), 
      &height,
      1);

  Global::md3dDevice->OMSetRenderTargets( 1, &mGBuffOccBlur.target, mDepthStencilView);


  //Draw quad covering screen
  UINT strides = sizeof( ScreenVertex );
	UINT offsets = 0;
	ID3D10Buffer* pBuffers[1] = { screenQuadVB };

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadPostBlur].mVertexLayout );
  Global::md3dDevice->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
	Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	mShaderManager.mShaderTable[indexShadPostBlur].mTech->GetPassByName( (LPCSTR)"V")->Apply( 0 );
	Global::md3dDevice->Draw( 4, 0 );
  mShaderManager.mShaderTable[indexShadPostBlur].SetResource( &(std::string("textCol")), 0 );
  mShaderManager.mShaderTable[indexShadPostBlur].mTech->GetPassByName( (LPCSTR)"V")->Apply( 0 );

	// VERTICAL  
  
  md3dDevice->OMSetRenderTargets( 1, &mGBuffOcc.target, mDepthStencilView);
  mShaderManager.mShaderTable[indexShadPostBlur].SetResource( &(std::string("textCol")), mGBuffOccBlur.resource );


  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadPostBlur].mVertexLayout );
  Global::md3dDevice->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
	Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	mShaderManager.mShaderTable[indexShadPostBlur].mTech->GetPassByName( (LPCSTR)"H")->Apply( 0 );
	Global::md3dDevice->Draw( 4, 0 );


  /////////////////////////
  //  Unbound resources
  mShaderManager.mShaderTable[indexShadPostBlur].SetResource( &(std::string("textCol")), 0 );
  mShaderManager.mShaderTable[indexShadPostBlur].mTech->GetPassByName( (LPCSTR)"H")->Apply( 0 );
}

void BaseDX::compositeDeferredFinal()
{
/////////////////////////////////////////////////////////
  //    Apply final composition
  md3dDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView);

  //  Set input textures  

  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textSSDO")), mGBuffOcc.resource );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textCol")), mGBuffCol.resource );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textLig")), mGBuffLig.resource );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textNor")), mGBuffNor.resource );  
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textPos")), mGBuffPos.resource );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  D3DXVECTOR3 fogCircCenter   = D3DXVECTOR3(00,0,0);
  float       fogCircRadius   =  20;
  float       fogYPlaneTop    =  10;
  float       fogYPlaneBottom = -50;

  mShaderManager.mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogCircCenter")),    (float*)&fogCircCenter  , 3 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogCircRadius")),    (float*)&fogCircRadius  , 1 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogYPlaneTop")),     (float*)&fogYPlaneTop   , 1 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogYPlaneBottom")),  (float*)&fogYPlaneBottom, 1 );

  //Draw quad covering screen
  UINT strides = sizeof( ScreenVertex );
	UINT offsets = 0;
	ID3D10Buffer* pBuffers[1] = { screenQuadVB };

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefComposition].mVertexLayout );
  Global::md3dDevice->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
	Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	mShaderManager.mShaderTable[indexShadDefComposition].mTech->GetPassByIndex( 0 )->Apply( 0 );
	// Composite the blurred model into the original image.
	Global::md3dDevice->Draw( 4, 0 );

  //  release
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("PermTexture")), 0 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textSSDO")), 0 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textCol")) , 0 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textLig")) , 0 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textNor")) , 0 );
  mShaderManager.mShaderTable[indexShadDefComposition].SetResource( &(std::string("textPos")) , 0 );
  mShaderManager.mShaderTable[indexShadDefComposition].mTech->GetPassByIndex( 0 )->Apply( 0 );	
  
}

//===========================================================================
/*!
    Deferred rendering: Update lighting buffer
*/
//===========================================================================
void BaseDX::applyLightsDeferred()
{
	// Bind the render target view and depth/stencil view to the pipeline.
  Global::md3dDevice->OMSetRenderTargets( 1, &mGBuffLig.target, mDepthStencilView);

  // clear light buffer
  Global::md3dDevice->ClearRenderTargetView(mGBuffLig.target, D3DXCOLOR(0,0,0,1) );
  Global::md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);

  //  Set input texture  
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textPos2")), mGBuffPos.resource );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textNor2")), mGBuffNor.resource );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textTan2")), mGBuffTan.resource );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textCol2")), mGBuffCol.resource );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textLightP")), mGBuffLightP.resource );
  
  mShaderManager.mShaderTable[indexShadDefLight].SetMatrix( &(std::string("mP")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefLight].SetMatrix( &(std::string("mV")), &Global::mView );  

  mShaderManager.mShaderTable[indexShadDefLight].SetRawValue( &(std::string("camPos")), (float*)&Global::mEyePos, 3 );


#ifndef __DP_DISABLE_LIGHTING
  /////////////////////////////////////////////////////////
  //    Draw light volumes

  // prepare dx 
  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefLight].mVertexLayout );
	Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
  ID3D10RasterizerState* rasterStateCCW;
  D3D10_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D10_RASTERIZER_DESC));
	cmdesc.FillMode = D3D10_FILL_SOLID;
	cmdesc.CullMode = D3D10_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	Global::md3dDevice->CreateRasterizerState(&cmdesc, &rasterStateCCW);
  
  Global::md3dDevice->RSSetState( rasterStateCCW );	

  //  Draw all lights
  if (nLights > maxDefLights ) nLights = maxDefLights ;
  for ( int i = 0 ; i < nLights ; ++i)
  //for ( int i =1 ; i < 10 ; ++i)
  {
    mShaderManager.mShaderTable[indexShadDefLight].SetRawValue( &(std::string("lPos")), (float *)&pLights[i].center, 3 );
    mShaderManager.mShaderTable[indexShadDefLight].SetRawValue( &(std::string("lCol")), (float *)&pLights[i].colour, 4 );
    mShaderManager.mShaderTable[indexShadDefLight].SetRawValue( &(std::string("lRad")), (float *)&pLights[i].radius, 1 );

    D3DXMatrixTranslation(  &Global::mWorld, 
                            pLights[i].center.x,
                            pLights[i].center.y,
                            pLights[i].center.z);

    D3DXMATRIX m;    
    float scale = pLights[i].radius;
    D3DXMatrixScaling(&m, scale, scale, scale);
    m = m*Global::mWorld;    

    mShaderManager.mShaderTable[indexShadDefLight].SetMatrix( &(std::string("mW")), &m );
    mShaderManager.mShaderTable[indexShadDefLight].mTech->GetPassByIndex( 0 )->Apply( 0 );
	  // Composite the blurred model into the original image.
    //Global::md3dDevice->Draw( 4, 0 );
    mSphere->display();
    //box.display();
  }


  //////////////////////
  //  Unbound resources
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textPos2")), 0 );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textNor2")), 0 );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textTan2")), 0 );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textCol2")), 0 );
  mShaderManager.mShaderTable[indexShadDefLight].SetResource( &(std::string("textLightP")), 0 );
  
  mShaderManager.mShaderTable[indexShadDefLight].mTech->GetPassByIndex( 0 )->Apply( 0 );
#endif
}

void BaseDX::drawLights	( PointLight* pLights, int* nLights )
{
 
}

void BaseDX::keybCleanAll()
{
  for (int i = 0; i < 256; i++ )
    keys[i] = false;
  keyReleased = false;
}

bool BaseDX::createRenderTarget ( DxRenderTarget& aRenderTarget,
                          int                         aFormat,
                          int                         aWidth,
                          int                         aHeight )
{
  aRenderTarget.texture  = NULL;
  aRenderTarget.target   = NULL;
  aRenderTarget.resource = NULL;

  // Create the render target texture
  D3D10_TEXTURE2D_DESC desc;
  ZeroMemory( &desc, sizeof(desc) );
  desc.Width = aWidth;
  desc.Height = aHeight;
  desc.MipLevels = 1;
  desc.ArraySize = 1;  
  desc.Format = DXGI_FORMAT(aFormat);
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D10_USAGE_DEFAULT;
  desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;

  HRESULT hr;
  hr = md3dDevice->CreateTexture2D( &desc, NULL, &aRenderTarget.texture );
  if ( FAILED(hr) ) return false;

  //  Create its render target
  D3D10_RENDER_TARGET_VIEW_DESC rtDesc;
  rtDesc.Format = desc.Format;
  rtDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
  rtDesc.Texture2D.MipSlice = 0;

  
  hr = md3dDevice->CreateRenderTargetView( aRenderTarget.texture, &rtDesc, &aRenderTarget.target);
  if ( FAILED(hr) ) return false;
 
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT(aFormat);
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;
  
  hr = md3dDevice->CreateShaderResourceView( aRenderTarget.texture, &srDesc, &aRenderTarget.resource );
  if ( FAILED(hr) ) return false;

  return true;  
}

void BaseDX::generatePermutationTexture()
{
  //  Create Vectors over a semisphere
  Rand::RandGen r(42);
  std::vector<int> perm = r.genPermutations( 1024 );
 
  D3D10_TEXTURE2D_DESC desc;
  desc.Width  = 32;
  desc.Height = 32;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R16_UINT;    
  desc.Usage = D3D10_USAGE_IMMUTABLE;
  desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;
  
  D3D10_SUBRESOURCE_DATA dataDesc;
  dataDesc.pSysMem = &perm[0];
  //dataDesc.SysMemPitch = mSsdoTexturePatternSize*sizeof(float)*3;
  dataDesc.SysMemPitch = 128;
  dataDesc.SysMemSlicePitch = 0;
  md3dDevice->CreateTexture2D(&desc, &dataDesc, &mPermTexture);
  
  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

  ZeroMemory( &descSRV, sizeof(descSRV) );
  descSRV.Format = DXGI_FORMAT_R16_UINT;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  descSRV.Texture2D.MipLevels = 1;
  descSRV.Texture2D.MostDetailedMip = 0;

  md3dDevice->CreateShaderResourceView( mPermTexture, &descSRV, &mPermTextureSRV ) ;
}

#ifdef _DP_UNIT_TESTS
void BaseDX::unitTests()
{
 float x;

 x = Interpolation::bias(0.2f, 0.0f);
 x = Interpolation::bias(0.2f, 0.1f);
 x = Interpolation::bias(0.2f, 0.2f);
 x = Interpolation::bias(0.2f, 0.5f);
 x = Interpolation::bias(0.2f, 0.6f);
 x = Interpolation::bias(0.2f, 0.8f);
 x = Interpolation::bias(0.2f, 0.9f);
 x = Interpolation::bias(0.2f, 1.0f);

 x = Interpolation::gain(0.2f, 0.0f);
 x = Interpolation::gain(0.2f, 0.1f);
 x = Interpolation::gain(0.2f, 0.2f);
 x = Interpolation::gain(0.2f, 0.5f);
 x = Interpolation::gain(0.2f, 0.6f);
 x = Interpolation::gain(0.2f, 0.8f);
 x = Interpolation::gain(0.2f, 0.9f);
 x = Interpolation::gain(0.2f, 1.0f);

 x = Interpolation::smootstep( 0.0f);
 x = Interpolation::smootstep( 0.1f);
 x = Interpolation::smootstep( 0.2f);
 x = Interpolation::smootstep( 0.5f);
 x = Interpolation::smootstep( 0.6f);
 x = Interpolation::smootstep( 0.8f);
 x = Interpolation::smootstep( 0.9f);
 x = Interpolation::smootstep( 1.0f);
 
}
#endif


namespace WinIO{

std::wstring s2ws(const std::string& s)
{
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
}

};