
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
	float3    gEyePosW;
	float3    camUp;
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

  float4 p0 = mul( float4( gIn[0].centerW , 1.0f), gViewProj);
  float4 p1 = mul( float4( gIn[1].centerW , 1.0f), gViewProj);

	float4 base[4];
  if ( p0.x/p0.w >= p1.x/p1.w )
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


  float halfHeight = 0.001f;

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
  float3 right = ( p1-p0 ).xyz;
	right = normalize(right);
	float3 look = cross( right, up);
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

		gOut.posH     = mul(v[i], W);
		gOut.posH     += base[i];
/*
		gOut.posH     = mul(v[i], WVP);
		gOut.posH     += mul(base[i], WVP);
*/
		gOut.normalW  = look;
		gOut.texC     = texC[i];
		gOut.primID   = primID; 
		
		triStream.Append(gOut);
	}

}

float4 PS(GS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	float cx = (pIn.texC.x ) - 0.5f;
	float cy = (pIn.texC.y )- 0.5f;
	cx = cx*cx;
	cy = cy*cy;
	float c = 0.5f - sqrt ( cx + cy );

//	float4 diffuse = float4 ( 1.0, 1.0, 1.0, c );
	float4 diffuse = float4 ( 1.0, 1.0, 1.0, c );

//float4 diffuse = float4 ( c, c, c, 0.2 );
//diffuse = float4 ( 1.0, 1.0, 1.0, 1.0 );
    
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
