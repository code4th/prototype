//--------------------------------------------------------------------------------------
// File: ssao.fx
//
// The effect file for the Screen Space Ambient Occlusion.
//--------------------------------------------------------------------------------------

//fxc ssao.fx /T fx_2_0 /Fo ssao.fxo

//ワールド、ビュー、射影座標変換マトリックス
float4x4 m_WVP;

//照明の方向ベクトル
float4 m_LightDir;

//環境光に対するマテリアル。0.0f～1.0fの値を持ち、1.0fのとき最も強くなる。
float4 m_Ambient = 0.0f;
int m_iFlag;

//****************************************************************
//バーテックスシェーダー
//****************************************************************
//バーテックスシェーダーからピクセルシェーダーへ渡すための構造体
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //頂点の座標
   float4 Col   : COLOR0;       //頂点カラー
   float4 depth : COLOR1 ;
   float3 normal : COLOR2 ;
   float2 Tex   : TEXCOORD0;    //テクセル座標
};
//バーテックスシェーダー
VS_OUTPUT VS( float4 Pos     : POSITION,   //頂点の座標
              float4 Normal  : NORMAL,     //法線ベクトル
              float2 Tex     : TEXCOORD0 ) //テクセル
{
   VS_OUTPUT Out = (VS_OUTPUT)0;
   
   //頂点の座標を行列変換する
   Out.Pos    = mul( Pos, m_WVP );
   
   //テクセルはそのまま格納する
   Out.Tex    = Tex;
   
   //照明を逆方向にする。(注意１)
   float3 L = -normalize( m_LightDir.xyz );
   
   //法線ベクトル。
   float3 N = normalize( Normal.xyz );

   //照明と法線からdot関数により、内積を計算する。またmax関数により頂点カラーの最小値を環境光に抑えるように設定する。(注意２)
   Out.Col = max( m_Ambient, dot(N, L) );
   Out.depth = Out.Pos;
   
   Out.normal = Normal.xyz;
   Out.normal = normalize(Out.normal);
   return Out;
}

//****************************************************************
//ピクセルシェーダー
//****************************************************************
sampler tex0 : register(s0) = sampler_state 
{ 
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
}; 

struct SP_OUTPUT
{
	float4 Color		: COLOR0;	// カラー
	float4 NormalDepth  : COLOR1;	// 法線と深度
};
SP_OUTPUT PS( VS_OUTPUT In )
{  
	SP_OUTPUT Out = (SP_OUTPUT)0;
   
	Out.Color = In.Col * tex2D( tex0, In.Tex );
//	Out.Color = In.Col;
	Out.NormalDepth.xyz = In.normal;
	Out.NormalDepth.w = In.depth.z/In.depth.w;
	return Out;
}


/*
uniform float totStrength = 1.38;
uniform float strength = 0.07;
uniform float offset = 18.0;
uniform float falloff = 0.000002;
uniform float rad = 0.006;
*/

float totStrength = 1.38;
float strength = 0.0007;
float offset = 18.0;
float falloff = 0.000002;
float rad = 0.03;

#define SAMPLES 16 // 10 is good
const float invSamples = 1.0/16.0;

sampler rayMap : register(s0) = sampler_state 
{ 
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
}; 
sampler normalMap : register(s1) = sampler_state
{
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler colorMap : register(s2) = sampler_state
{
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct SSPS_OUTPUT
{
    float4 RGBColor	: COLOR0;  // Pixel color
};
//--------------------------------------------------------------------------------------
// ピクセルシェーダ メイン http://www.gamerendering.com/2009/01/14/ssao/ の移植(微修正)
//--------------------------------------------------------------------------------------
SSPS_OUTPUT SSAO( float2 uv		: TEXCOORD0 )
{

    SSPS_OUTPUT Output;

	// 現在フラグメントのピクセル
	float4 currentPixel = tex2D(normalMap, uv);

	// 現在フラグメントの法線
	float3 currentNormal = currentPixel.xyz * 2.0f - 1.0f;
	// 現在フラグメントの深度
	float currentDepth = currentPixel.a;
	// 現在フラグメントの座標
	float3 currentPos = float3(uv.x, uv.y, currentDepth);

	// サンプリング半径。手前ほど周りを見る距離を大きくする(パースの補正)
	float radD = rad / currentDepth;

	float ao = 0.0;
	float4 gi = 0.0;
	for(int i=0; i<SAMPLES;++i)
	{
		float3 fres = tex2D(rayMap, float2(float(i)/16.f,0)).xyz*2.f-1.f;
		float3 ray = radD * fres;

		// 光線が半球の外(反対)なら反転する
		float2 sample = currentPos + sign(dot(ray,currentNormal) )*ray;
		// チェックするフラグメントを取得
		float4 samplePixel = tex2D(normalMap, sample.xy);
		// チェックするフラグメントの法線を取得
		float3 sampleNormal = samplePixel.xyz * 2.0f - 1.0f;
		// チェックするフラグメントの色
		float4 sampleColor = tex2D(colorMap, sample.xy);
		// チェックするフラグメントの座標
		float3 samplePos = float3(sample.x, sample.y, samplePixel.a);


		// 深度値の差
		float dDepth = currentDepth - samplePixel.a;

		// 法線の差
		float dNormal = (1.0 - dot(normalize(sampleNormal), normalize(currentNormal)));

		//    前後関係での遮蔽有無         角度による遮蔽           前後関係での遮蔽変化をソフトに
		ao += step(falloff, dDepth) * dNormal * (1.0 - smoothstep(falloff, strength, dDepth));
//		float invLen = 1.f/length(currentPos-samplePos);
		gi += ao * sampleColor;
	}

	// AO項の計算。遮蔽されているほどblが大きいので、暗くなる。
	ao = totStrength * ao * invSamples;
	gi = 0.2 * gi * invSamples;
//	Output.RGBColor = tex2D(colorMap, uv)*(1-gi*(1.0-ao));

	if(1==m_iFlag)
		Output.RGBColor = tex2D(colorMap, uv);
	if(0==m_iFlag)
		Output.RGBColor = tex2D(colorMap, uv)*(1-gi*(1.0-ao));

	return Output;
}

//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene0
{
    pass P0
    {
	    VertexShader = compile vs_3_0 VS();
		PixelShader  = compile ps_3_0 PS();   
    }
    pass P1
    {
        PixelShader  = compile ps_3_0 SSAO();
    }
}