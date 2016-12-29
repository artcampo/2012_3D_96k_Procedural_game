#include "simplex.fx"
/*
const float trailSamples      = 100.0f;
const float trailSamplesLine  = 98.0f;
*/
/*
const float trailSamples      = 40.0f;
const float trailSamplesLine  = 38.0f;
const float trailHead         = 10.0f;
const float trailAlmostHalf   = 6.0f;
const float trailHalf   = 5.0f;
*/
const float trailSamples      = 100.0f;
const float trailSamplesLine  = 98.0f;
const float trailHead         = 40.0f;
const float trailAlmostHalf   = 22.0f;
const float trailHalf   = 20.0f;


BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};


RasterizerState rs_noCull
{
CullMode=None;
};

DepthStencilState StencilState_NoDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = Less;


};

cbuffer cbPerFrame
{
	float3    camUp;
	float3    gEyePosW;
	float4x4  gViewProj; 
	float4x4  View; 
	float4x4  invView; 
};
 

struct VS_IN
{
	float3 centerW : POSITION;

};

struct VS_OUT
{
	float3 centerW : POSITION;

};

struct GS_OUT
{
    float4 posH    : SV_POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
    uint primID    : SV_PrimitiveID;
};

 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Just pass data into geometry shader stage.
	vOut.centerW  = vIn.centerW;


	return vOut;
}

[maxvertexcount(6)]
void GS(line VS_OUT gIn[2], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GS_OUT> triStream)
{	
  float3 tangentWS = gIn[1].centerW - gIn[0].centerW;
/*  float2 tangentSS = tangent.xy / tangent.z;
  tangentSS = tangentCS.xy * (0.5f, -0.5f);
  tangentSS = tangentCS.xy + (0.5, 0.5);
*/
  float3 normal = float3( 0.0f, 1.0f, 0.0f );

  float4 p0 = mul( float4( gIn[0].centerW , 1.0f), gViewProj);
  float4 p1 = mul( float4( gIn[1].centerW , 1.0f), gViewProj);

	float4 base[4];
  if ( p0.x >= p1.x)
  {
    base[0] = float4( gIn[0].centerW , 1.0f);
    base[1] = float4( gIn[1].centerW , 1.0f);
    base[2] = float4( gIn[0].centerW , 1.0f);
    base[3] = float4( gIn[1].centerW , 1.0f);
  }
  else
  {
    base[0] = float4( gIn[1].centerW , 1.0f);
    base[1] = float4( gIn[0].centerW , 1.0f);
    base[2] = float4( gIn[1].centerW , 1.0f);
    base[3] = float4( gIn[0].centerW , 1.0f);
  }


  float halfHeight = 2.5f;

	float4 v[4];
	v[0] = float4(0.0f, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(0.0f, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(0.0f, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(0.0f, +halfHeight, 0.0f, 1.0f);
	float2 texC[4];
	texC[0] = float2(0.0f, 1.0f);
	texC[1] = float2(1.0f, 1.0f);
	texC[2] = float2(0.0f, 0.0f);
	texC[3] = float2(1.0f, 0.0f);

  //  Transformation matrix
  float3 up = mul ( float4(0,1,0,0), invView).xyz;
	up = normalize(up);
  float3 right = mul ( float4(-1,0,0,0), invView).xyz;
	right = normalize(right);
	float3 look = mul ( float4(0,0,-1,0), invView).xyz;
	look = normalize(look);	
	float4x4 W;
	W[0] = float4(right,              0.0f);
	W[1] = float4(up,                 0.0f);
	W[2] = float4(look,               0.0f);
	W[3] = float4( 0.0f,0.0f,0.0f,1.0f);
	float4x4 WVP = mul(W,gViewProj);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GS_OUT gOut;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{

		gOut.posH     = mul(v[i], WVP);
		gOut.posH     += mul(base[i], WVP);
		gOut.normalW  = look;
		gOut.texC     = texC[i];
		gOut.primID   = primID; 
		
		triStream.Append(gOut);
	}
}

/////////////////////////////////////////////////////////////////////////
/*
    PShader for a particle's trail, which is represented as a line,
    coded in the GShader as a consecution of quads.
    -textUV, this quad's UV
    -primID, the id of the quad. 0 <= primID <= trailSamplesLine
*/
/////////////////////////////////////////////////////////////////////////
float particleTrailLine ( float2 textUV, float primID )
{
  float PI = 3.1415f;

	float cx = (textUV.x * PI) ;
	float cy = (textUV.y * PI) ;


	float c = sin (cy);
  float c2 = sin ( cy*2.0f);

  //c = 1.0f - sin(cy*2.0);


  // Intensity proportional to length
  float cola = 1.0f - ( primID/trailSamplesLine );
  cola = 1.0f - exp ( -(cola * 1.2f) );
  cola *= 0.75f;

  float cabeza = ( (primID )/trailSamplesLine );
  cabeza = 1.0f - exp ( -(cabeza * 1.5f) );
  cabeza *= 2.75f;




float aging;

  float t = 1.0f - (primID / trailSamplesLine );
  aging = lerp ( cola, cabeza, t );



  c = c * cola * c2*cabeza;
  c = c * cola;
  

    float p = primID - trailHalf;
    p *= 0.1f;
    p = p*p;
    p = sqrt(p);
    p = 1.0f - p;
  p*=0.2f;
  p = 1.0f-exp(-p*0.2);

  p = clamp(p,0,1);

  c = clamp(c,0,1);
  c = c + 1.5*p*cabeza - 1.2*p*cola;


//  c += p*cabeza;
//  c = p*p;
  


  return c;
}

float4 PS(GS_OUT pIn) : SV_Target
{

  float c = particleTrailLine ( pIn.texC, pIn.primID );

	float4 diffuse = float4 ( 1.0, 1.0, 1.0, c );

  return diffuse;
}
 
technique10 ParticleTest
{
    pass P0
    {
        SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( StencilState_NoDepth, 0 );

        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );
        SetRasterizerState(rs_noCull);
    }
}
