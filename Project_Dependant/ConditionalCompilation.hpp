#ifndef _CONDITIONAL_COMPILATION_HPP
#define _CONDITIONAL_COMPILATION_HPP

////////////////////////////////////////////////////////////////////////
//  used for track graphic debug
////////////////////////////////////////////////////////////////////////

//#define GBP_GFXDEBUG_TRACK
//#define GBP_DISABLE_ARCH_CREATION
//#define GBP_DISABLE_BUMP_CREATION
//#define GBP_ENABLE_DEBUG_GFX_COLLISION_SPHERES

////////////////////////////////////////////////////////////////////////
//  used for fast render (debug)
////////////////////////////////////////////////////////////////////////

//#define _GBP_FAST_RENDER
#define GBP_DISABLE_FOG_DISPLAY
//#define __DP_DISABLE_LIGHTING
//#define GBP_RENDER_WIREFRAME
#define GBP_DISABLE_RENDER_END_OF_TRACK_RECTANGLE
#define GBP_DISABLE_RENDER_COLLIDERS_RECT
#define _GBP_DISABLE_FUSTRUM_CULLING_  

////////////////////////////////////////////////////////////////////////
//  used to disable log
////////////////////////////////////////////////////////////////////////


#define GBP_LOG_TRACK_CREATION

////////////////////////////////////////////////////////////////////////
//  used for physics
////////////////////////////////////////////////////////////////////////

//  gameBpApp
//#define _DP_PHYSICS_DEBUG_FIXED_TIME_STEP
//#define GBP_DISABLE_LIGHTS

#define _BP_EXTRA_LOG

//#define _DP_LOFT_DEBUG_LOG

//#define _BP_DEBUG_TRACK_CREATION


//#define _DEBUG_DISABLE_INTERSECTION
//#define _DEBUG_ENABLE_TRACK_FOR_INTERSECTION_TEST

//#define DISABLE_CAM
  //#define CAMERA_FOR_SSDO_TEST

//#define GBP_RENDER_TRACK_ON_LINES
//#define GBP_RENDER_WIREFRAME

#define _GBP_DISABLE_METROPOLIS_RENDER
//#define _DPBP_DISABLE_BORDER_SLICE



//UNUSED:
//#define ___DP_DEBUG_SSAO__
//#define ___DP_DISABLE_SSAO


//  game

#endif