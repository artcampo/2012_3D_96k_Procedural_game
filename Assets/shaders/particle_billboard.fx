
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
  float  size    : PSIZE;
  float4 color   : COLOR;

};

struct VS_OUT
{
	float3 centerW : POSITION;
  float  size    : PSIZE;
  float4 color   : COLOR;
};

struct GS_OUT
{
    float4 posH    : SV_POSITION;
    float4 color    : COLOR;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
    uint primID    : SV_PrimitiveID;
};

 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Just pass data into geometry shader stage.
	vOut.centerW  = vIn.centerW;
	vOut.color  = vIn.color;
  vOut.size = vIn.size;


	return vOut;
}

[maxvertexcount(4)]
void GS(point VS_OUT gIn[1], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GS_OUT> triStream)
{	
	//
	// Compute 4 triangle strip vertices (quad) in local space.
	// The quad faces down the +z axis in local space.
	//
	float halfWidth  = 0.5f*gIn[0].size;
	float halfHeight = 0.5f*gIn[0].size;
	
	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
	//
	// Compute texture coordinates to stretch texture over quad.
	//
	float2 texC[4];
	texC[0] = float2(0.0f, 1.0f);
	texC[1] = float2(1.0f, 1.0f);
	texC[2] = float2(0.0f, 0.0f);
	texC[3] = float2(1.0f, 0.0f);
	//
	// Compute world matrix so that billboard is aligned with
	// the y-axis and faces the camera.
	//


/*
  float4x4 inV;
	inV[0] = float4(v[0][0],v[1][0],v[2][0],              0.0f);
	inV[1] = float4(v[0][1],v[1][1],v[2][1],                 0.0f);
	inV[2] = float4(v[0][2],v[1][2],v[2][2],               0.0f);
	inV[3] = float4(gIn[0].centerW, 1.0f);
*/
  float3 up = mul ( float4(0,1,0,0), invView).xyz;
//up = camUp;
	up = normalize(up);
/*
	float3 look = gEyePosW - gIn[0].centerW;

	look = normalize(look);

	float3 right = cross(up, look);
*/
float3 right = mul ( float4(-1,0,0,0), invView).xyz;
	right = normalize(right);
	float3 look = mul ( float4(0,0,-1,0), invView).xyz;
	look = normalize(look);
	
	float4x4 W;
	W[0] = float4(right,              0.0f);
	W[1] = float4(up,                 0.0f);
	W[2] = float4(look,               0.0f);
	W[3] = float4(gIn[0].centerW, 1.0f);

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
		gOut.posW     = mul(v[i], W);
		gOut.normalW  = look;
		gOut.texC     = texC[i];
		gOut.primID   = primID;
    gOut.color = gIn[0].color;
		
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
	float4 diffuse = float4 ( pIn.color.xyz, c );

//float4 diffuse = float4 ( c, c, c, 0.2 );
    
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

    }
}
