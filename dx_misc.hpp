#ifndef _DX_MISC_HPP_
#define _DX_MISC_HPP_

#ifndef _WIN32_WINNT
  //#define _WIN32_WINNT   0x0501 // Windows XP
  //#define _WIN32_WINNT   0x0502 // Windows XP SP2
  //#define _WIN32_WINNT   0x0600 // Windows Vista
  #define _WIN32_WINNT   0x0601 // Windows 7
#endif

#include <d3dx10.h>
#include <dxerr.h>
#include <windowsx.h>
#include <string>
#undef max

//---------------------------------------------
// MACROS
//---------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release();x = NULL; } }

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 


//---------------------------------------------
// AUXILIAR STRUCTS
// Letter at the end
// W: World
// H: Homogeneous clip space
//---------------------------------------------

struct Vertex
{
  D3DXVECTOR3 pos;  // Position
};


struct VertexPos
{
  D3DXVECTOR3 pos;  // Position
};

struct VertexPosCol
{
  D3DXVECTOR3 pos;  // Position
  D3DXCOLOR   col;
};


struct VertexPosNor
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
};


struct VertexPosNorCol
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
  D3DXCOLOR   col;
};



struct VertPNT
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
  D3DXVECTOR3 tan;  // Tangent
};

struct VertPNTUV
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
  D3DXVECTOR3 tan;  // Tangent
  D3DXVECTOR2 uv;   // Texture Coordinates
};


struct ColorVertex
{
  D3DXVECTOR3 pos;  // Position
  D3DXCOLOR   color;  
};

struct  PointLight{
    D3DXVECTOR3   center;
    float         radius;       //World coordinates
    D3DXVECTOR4   colour;
};

struct ScreenVertex
{
    D3DXVECTOR4 pos;
    D3DXVECTOR2 tex;
};

struct ParticleVertex
{
	D3DXVECTOR3 centerW;
	D3DXVECTOR2 sizeW;
  D3DXCOLOR   color;  
};

struct VertexPointPSC
{
  D3DXVECTOR3 centerW;
  D3DXVECTOR2 sizeW;
  D3DXCOLOR   color;
};

struct VertexPC
{
  D3DXVECTOR3 pos;  // Position
  D3DXCOLOR   col;
};

//*****************************************************************************
// Constants
//*****************************************************************************

const D3DXCOLOR GREY(0.8f, 0.8f, 0.8f, 0.8f);
const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);

enum keysDefined
{KESC, KBACK, KINTRO, KUP, KDOWN, KRIGHT, KLEFT, KSHIFT, 
KSPACE, KW, KA, KS, KD, KU, KH, KJ, KK, 
KI,KT,KG,KY, KO,KM,KN,K_NON_EXISTANT};

const UINT16   SCREEN_W = 1024;
const UINT16   SCREEN_H = 768;

//*****************************************************************************
// Quick functions
//*****************************************************************************

// Returns random float in [0, 1).
D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b).
D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

#endif 

