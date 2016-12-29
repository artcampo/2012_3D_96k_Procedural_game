
Texture2D textPos;
Texture2D textNor;
Texture2D textTan;
Texture2D textCol;
Texture2D textLig;

Texture2D textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState samLinearWrap
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
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
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Output VS_FINAL_COMPOSITION( VS_Input Input )
{
    VS_Output Output;

      Output.Pos = Input.Pos;
//Output.Pos = float4 ( Input.Tex.x*2.0f - 1.0f, 1.0f - Input.Tex.y*2.0f, 0.5f, 1.0f);


    Output.Tex = Input.Tex;

    return Output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    
  const float sampleRadius  = 10.0;
  const float depthBias     = 0.01;
  const float singularity   = 10.0;
  const float strength      = 0.05;
  const int   patternSize   = 16;
  static const float PI = 3.14159265f;
	const float bounceSingularity = 3.0;
	const float bounceStrength = 0.025f;


    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
    float2 vSamplePosition;
    
    vSamplePosition = Input.Tex;

    float4 pos      = textPos.Sample( samLinear, vSamplePosition);
    float3 normal1  = textNor.Sample( samLinear, vSamplePosition).xyz;


    float3 N      = normalize( normal1 );
    float3 Tan = textTan.Sample( samLinear, vSamplePosition).xyz;
    Tan    = normalize(Tan);
    float3 BiTan  = normalize(cross(N, Tan));

/*
    float3x3  matLocal = float3x3 ( N.x, Tan.x, BiTan.x, 
                                    N.y, Tan.y, BiTan.y, 
                                    N.z, Tan.z, BiTan.z );
*/
    float3x3  matLocal = float3x3 ( Tan.x, N.x, BiTan.x, 
                                    Tan.y, N.y, BiTan.y, 
                                    Tan.z, N.z, BiTan.z );		

    float     ambientOcclusion    = 0.0;
    float3    directRadianceSum   = float3 ( 0.0, 0.0, 0.0 );
    float3    occRadianceSum      = float3 ( 0.0, 0.0, 0.0 );
    
    int patternIndex = int(vSamplePosition.x*1024) % patternSize +
                      (int(vSamplePosition.y*748) % patternSize) * patternSize;
    float patternSizeSquareInverse = 1.0f/float(patternSize*patternSize);

    for ( int i = 0; i < 8 ; ++i )
      {
      vColor.xyz = textSeed.Sample( samLinearWrap, float2( i, patternIndex )*patternSizeSquareInverse ).xyz;



      }


vColor.xyz = textSeed.Sample( samLinearWrap, vSamplePosition*patternSize ).xyz;

  
//  vColor.xyz = textSeed.Sample( samLinear, float2( texU, texV ) ).xyz;
//  vColor.xyz = (patternIndex / float(patternSize*patternSize))* float3(1.0, 1.0, 1.0 );

/*
	  vColor.xyz *= radianceSum;
*/


    vColor.xyz = textCol.Sample( samLinear, vSamplePosition).xyz;    
    //vColor.xyz *= vertexLig.xyz;
vColor.xyz += vertexLig.xyz;


//  vColor.xyz = vertexLig.xyz;


	vColor.a = 1.0;
	return vColor;



}

//--------------------------------------------------------------------------------------
technique10 SSDO
{

    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}
