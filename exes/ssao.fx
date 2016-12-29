Texture2D textPos;
Texture2D textNor;
Texture2D textTan;
Texture2D textCol;
Texture2D textLig;

Texture1D <float3> textRandDir;

float4x4 View     : VIEW;
float4x4 ViewProj : VIEWPROJECTION;

float   g_Radius;
float   g_TanBias;
float2  g_InvFocal;
float2  g_Focal;
float   g_FarPlane;
float   g_NearPlane;
float2  g_Resolution;
float2  g_InvResolution;


RasterizerState rs_noCull
{
CullMode=None;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_Input
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;              
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

float3 fetch_normal( float2 uv )
{
  float4  nWS   = float4( textNor.Sample( samLinear, uv).xyz, 0 );
  float4  nVS   = mul( nWS, View );
  return  nVS.xyz;
}

float3  tangent_plane( float2 uv, float4 plane )
{
  float4  pWS   = float4( textPos.Sample( samLinear, uv).xyz, 1 );
  float4  pVS   = mul( pWS, View );
  float   depth = pVS.z;
  float2  uvES  = uv * float2(2,-2) - float2(1,-1);  
  float3  V     =  float3( uvES*depth*g_InvFocal, depth );
  float   NdotV = dot(plane.xyz,V);
  if ( NdotV < 0 ) V *= plane.w/NdotV;
  return V;
}

float3  fetch_ES ( float2 uv )
{
  float4  pWS   = float4( textPos.Sample( samLinear, uv).xyz, 1 );
  float4  pVS   = mul( pWS, View );
  float   depth = pVS.z;

  float2  uvES  = uv * float2(2,-2) - float2(1,-1);  
  return  float3( uvES*depth*g_InvFocal, depth );
}

float3 minDifference ( float3 v1, float3 v2)
{
  if ( length(v1) < length(v2) ) 
    return v1;
  else
    return v2;

}

float falloff ( float d )
{
  return 1 - 0.1*d*d;
}

float2 snapCoord ( float2 v )
{
  return (round(v*g_Resolution) -0.5)*g_InvResolution;
}

float2 snapOffset ( float2 v )
{
  return (round(v*g_Resolution) )*g_InvResolution;
}

float2 rotate_direction( float2 v1, float2 v2 )
{
  return float2( v1.x*v2.y - v1.y*v2.x, v1.y*v2.y + v1.x*v2.x );
}

float tangent( float3 v )
{
  return -v.z / length( v.xy );
}

float tangent( float3 v1, float3 v2 )
{
  return (v1.z - v2.z ) / length( v2.xy - v1.xy );
}

float tan_to_sin ( float angle )
{
  return angle * rsqrt(angle*angle + 1);
}

float hbao( float3 P, float2 uv0, float2 dUV, float3 dPdu, float3 dPdv, float numSteps, float jitter)
{
  float2 uv     = uv0 + snapOffset( jitter*dUV );
  dUV           = snapOffset( dUV );
  
  float3  T     = dUV.x * dPdu + dUV.y * dPdv;
  float tanh    = tangent(T) + 0.00001;
  float sinh    = tan_to_sin(tanh);
  float ao      = 0;
  
  for ( int i = 0; i < numSteps; ++i )
  {
    uv        += dUV*jitter;
    float2  snapUV  = snapCoord( uv );
    float3  S = fetch_ES( snapUV );
    float   d = length( P - S );
    if ( d < g_Radius )
    {
      float   tans = tangent(P, S);
      if ( tans > tanh )
      {
        float sins;
        sins    = tan_to_sin(tans);
        ao      += falloff(d/g_Radius)*(sins - sinh);
        tanh    = tans;
        sinh    = sins;      
      }
    }
  }
  return ao;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Output VS_FINAL_COMPOSITION( VS_Input Input )
{
    VS_Output Output;
    Output.Pos = Input.Pos;
    Output.Tex = Input.Tex;

    return Output;
}

float rand(float2 co){
    return frac(sin(dot(co.xy ,float2(12.9898,78.233))) * 43758.5453);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    

  static const float PI = 3.14159265f;

  float3 pES    = fetch_ES( Input.Tex );
  
     /*
  float3 N = fetch_normal( Input.Tex );
  float4 plane = float4( N, dot(pES,N) );
  float3 ptES = tangent_plane( Input.Tex + float2( 0,  g_InvResolution.y ), plane );
  float3 pbES = tangent_plane( Input.Tex + float2( 0, -g_InvResolution.y ), plane );
  float3 plES = tangent_plane( Input.Tex + float2( -g_InvResolution.x, 0 ), plane );
  float3 prES = tangent_plane( Input.Tex + float2(  g_InvResolution.x, 0 ), plane );                      
*/
  float3 ptES = fetch_ES( Input.Tex + float2( 0,  g_InvResolution.y ) );
  float3 pbES = fetch_ES( Input.Tex + float2( 0, -g_InvResolution.y ) );
  float3 plES = fetch_ES( Input.Tex + float2( -g_InvResolution.x, 0 ) );
  float3 prES = fetch_ES( Input.Tex + float2(  g_InvResolution.x, 0 ) );    
  
  float3 dPdu = minDifference( prES - pES, pES - plES );
  float3 dPdv = minDifference( ptES - pES, pES - pbES );
  dPdv *= ( g_InvResolution.x * g_Resolution.y) ;

  float2 stepSize = float2( g_InvResolution.x, g_InvResolution.y );
  float numSteps = 3;
  stepSize *= 8;

   stepSize = 0.5 * g_Radius * g_Focal / pES.z;
   numSteps = min  ( 4, 
                          min ( stepSize.x * g_Resolution.x, 
                                stepSize.y * g_Resolution.y )
                        );
  if ( numSteps < 1 )  return 1;

  stepSize = stepSize / (numSteps + 1);

  //stepSize = stepSize / (numSteps + 1);

  int2 pos        = Input.Tex* g_Resolution;

  pos             = int2( int(pos.x & 63), int(pos.y & 63) );
  float3 randDir  = textRandDir.Load( int2(pos.x + 64*pos.y, 0 ) ).xyz;

  int numDirections = 8;
  float ao = 0;
  for ( int i = 0; i < numDirections ; ++i )
    {
      float angle = 2*PI*float(i) / float(numDirections);
      float2 dUV  = float2( cos(angle), sin(angle ) );
      dUV = rotate_direction ( dUV, randDir.xy );
      dUV *= stepSize;
 
      ao   += hbao( pES, Input.Tex, dUV, dPdu, dPdv, numSteps, randDir.z );

    }


  float finalAo = 1 - ( ao / numDirections);
  
  
  float4 color  = float4( finalAo, finalAo, finalAo, 1 );
    
//  float4 color = float4( dPdu+dPdv,1);
/*
  float c = dPdv.y;
color = float4( c,c,c,1);
color = float4( dPdu+dPdv,1);
*/
	return color;
}

//--------------------------------------------------------------------------------------
technique10 SSAO
{

    pass P0
    {
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState(rs_noCull);
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}
