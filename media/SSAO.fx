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


float totStrength = 1.38;
float strength = 0.07;
float offset = 18.0;
float falloff = 0.000002;
float rad = 0.006;

#define SAMPLES 16 // 10 is good
const float invSamples = 1.0/16.0;

sampler rnm : register(s0);
sampler normalMap : register(s1);

// 単位球内のランダムなベクトル
#if SAMPLES == 16
float3 pSphere[16] = {
	float3(0.53812504, 0.18565957, -0.43192),
	float3(0.13790712, 0.24864247, 0.44301823),
	float3(0.33715037, 0.56794053, -0.005789503),
	float3(-0.6999805, -0.04511441, -0.0019965635),
	float3(0.06896307, -0.15983082, -0.85477847),
	float3(0.056099437, 0.006954967, -0.1843352),
	float3(-0.014653638, 0.14027752, 0.0762037),
	float3(0.010019933, -0.1924225, -0.034443386),
	float3(-0.35775623, -0.5301969, -0.43581226),
	float3(-0.3169221, 0.106360726, 0.015860917),
	float3(0.010350345, -0.58698344, 0.0046293875),
	float3(-0.08972908, -0.49408212, 0.3287904),
	float3(0.7119986, -0.0154690035, -0.09183723),
	float3(-0.053382345, 0.059675813, -0.5411899),
	float3(0.035267662, -0.063188605, 0.54602677),
	float3(-0.47761092, 0.2847911, -0.0271716)
};
#elif SAMPLES == 8
const float3 pSphere[8] = {
	float3(0.24710192, 0.6445882, 0.033550154),
	float3(0.00991752, -0.21947019, 0.7196721),
	float3(0.25109035, -0.1787317, -0.011580509),
	float3(-0.08781511, 0.44514698, 0.56647956),
	float3(-0.011737816, -0.0643377, 0.16030222),
	float3(0.035941467, 0.04990871, -0.46533614),
	float3(-0.058801126, 0.7347013, -0.25399926),
	float3(-0.24799341, -0.022052078, -0.13399573)
};
#elif SAMPLES == 12
const float3 pSphere[12] = {
	float3(-0.13657719, 0.30651027, 0.16118456),
	float3(-0.14714938, 0.33245975, -0.113095455),
	float3(0.030659059, 0.27887347, -0.7332209),
	float3(0.009913514, -0.89884496, 0.07381549),
	float3(0.040318526, 0.40091, 0.6847858),
	float3(0.22311053, -0.3039437, -0.19340435),
	float3(0.36235332, 0.21894878, -0.05407306),
	float3(-0.15198798, -0.38409665, -0.46785462),
	float3(-0.013492276, -0.5345803, 0.11307949),
	float3(-0.4972847, 0.037064247, -0.4381323),
	float3(-0.024175806, -0.008928787, 0.17719103),
	float3(0.694014, -0.122672155, 0.33098832)
};
#else
const float3 pSphere[10] = {
	float3(-0.010735935, 0.01647018, 0.0062425877),
	float3(-0.06533369, 0.3647007, -0.13746321),
	float3(-0.6539235, -0.016726388, -0.53000957),
	float3(0.40958285, 0.0052428036, -0.5591124),
	float3(-0.1465366, 0.09899267, 0.15571679),
	float3(-0.44122112, -0.5458797, 0.04912532),
	float3(0.03755566, -0.10961345, -0.33040273),
	float3(0.019100213, 0.29652783, 0.066237666),
	float3(0.8765323, 0.011236004, 0.28265962),
	float3(0.29264435, -0.40794238, 0.15964167)
};
#endif

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
//   float2 Tex   : TEXCOORD0;    //テクセル座標
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
//   Out.Tex    = Tex;
   
   //照明を逆方向にする。(注意１)
   float3 L = -normalize( m_LightDir.xyz );
   
   //法線ベクトル。
   float3 N = normalize( Normal.xyz );

   //照明と法線からdot関数により、内積を計算する。またmax関数により頂点カラーの最小値を環境光に抑えるように設定する。(注意２)
   Out.Col = max( m_Ambient, dot(N, L) );
   Out.depth = Out.Pos;
   
   float4 normal = mul( Normal, m_WVP );
   Out.normal = normalize(normal);
   return Out;
}

//****************************************************************
//ピクセルシェーダー
//****************************************************************
float4 PS( VS_OUTPUT In ): COLOR0
{  
   float4 Out = (float4)0;
   
   //色情報をCol1に格納する
//   Out = In.Col * tex2D( tex0, In.Tex );
//   Out = In.Col;
//   Out.xyz = In.normal;

	Out = float4(In.normal.x,In.normal.y,In.normal.z,In.depth.z/In.depth.w);
   return Out;
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct SSVS_OUTPUT
{
    float4 Position	: POSITION;
//    float2 uv  		: TEXCOORD0;
};
struct SSPS_OUTPUT
{
    float4 RGBColor	: COLOR0;  // Pixel color
};
SSVS_OUTPUT RenderSceneVS( float4 vPos : POSITION )
{
    SSVS_OUTPUT Output;
    Output.Position = vPos;
 //   Output.uv.x = (vPos.x + 1.0f) * 0.5f;
 //   Output.uv.y = 1.0f - (vPos.y + 1.0f) * 0.5f;

    return Output;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダ メイン http://www.gamerendering.com/2009/01/14/ssao/ の移植(微修正)
//--------------------------------------------------------------------------------------
SSPS_OUTPUT RenderScenePS0(
	float2 uv		: TEXCOORD0
)
{
    SSPS_OUTPUT Output;

	// grab a normal for reflecting the sample rays later on
	// 後でサンプル光線を反射させるための法線を
	// rnm :ランダムノーマルマップのサンプラ。ランダムノーマルマップはサンプリングをランダムにするために使う。
	// offset : ランダムノーマルマップが描画サイズより小さいことによって同じピクセルがサンプルされるのを防ぐ。
	float3 fres = normalize((tex2D(rnm, uv*offset).xyz*2.0) - 1.0f);

	// RGBに法線が、aに0～1のデプスが入っている
	float4 currentPixelSample = tex2D(normalMap, uv);

	float currentPixelDepth = currentPixelSample.a;

	// get the normal of current fragment
	// 現在フラグメントの法線
	float3 norm = currentPixelSample.xyz * 2.0f - 1.0f;


	float bl = 0.0;
	// adjust for the depth ( not shure if this is good..)
	// サンプリング半径。手前ほど周りを見る距離を大きくする(パースの補正)
	float radD = rad / currentPixelDepth;
	//float radD = rad;

	float3 ray, occNorm;
	float2 se;
	float occluderDepth, depthDifference, normDiff;

	for(int i=0; i<SAMPLES;++i)
	{
		// get a vector (randomized inside of a sphere with radius 1.0) from a texture and reflect it
		// テクスチャから取得した(半径1.0の球内に収まったランダムな)ベクトルに反射させる
		// なんでfresをそのままrayとして使わないんだろ。長さもバラけさせたいからかな。(fresは正規化されてる)
		// radDは深度値の偏りの補正。
		ray = radD * reflect(pSphere[i], fres);

		// if the ray is outside the hemisphere then change direction
		// 光線が半球の外(反対)なら反転する
		// 反対だとdotが負値を返すのでsignが-1を返す
		se = uv + sign(dot(ray,norm)) * ray * float2(1.0f, -1.0f);

		// get the depth of the occluder fragment
		// 遮蔽するフラグメントを取得
		float4 occluderFragment = tex2D(normalMap, se.xy);

		// get the normal of the occluder fragment
		// 遮蔽するフラグメントの法線を取得
		occNorm = occluderFragment.xyz * 2.0f - 1.0f;

		// if depthDifference is negative = occluder is behind current fragment
		// 正：遮蔽フラグメントは現在のフラグメントより前 →遮蔽されてる
		// 負：遮蔽フラグメントは現在のフラグメントより奥 →遮蔽されてない
		depthDifference = currentPixelDepth - occluderFragment.a;

		// calculate the difference between the normals as a weight
		// 遮蔽フラグメントと現在フラグメントでの法線の角度の差を計算
		// 同方向なら1、反対なら0
		// 同方向を向いていたら遮蔽されていないはず、角度がおおきければ遮蔽されているはず、ということ
		normDiff = (1.0 - dot(normalize(occNorm), normalize(norm)));

		// the falloff equation, starts at falloff and is kind of 1/x^2 falling
		// 減衰式。1/x^2 な感じで減衰する。
		// step : step(edge, x) 0.0 if x < edge, else 1.0
		//        depthDifference が falloff より小さければ 0、そうでなければ 1。
		//        →ある程度以上手前にあれば遮蔽
		// smoothstep : depthDifference が falloff 以下なら 0、strength 以上なら 1、そうでなければ間を補間
		//        →1.0-smoothstep(...) で、
		//          遮蔽フラグメントが現在フラグメントより少し前の場合、ソフトに暗くする。
		//          また、遮蔽フラグメントが手前過ぎる場合は遮蔽と見なさない。
		//          (falloffはstepの方で前後関係による遮蔽判定により使われているので、smoothstepの方では補間の開始位置という程度の意味)
		//
		//    前後関係での遮蔽有無         角度による遮蔽           前後関係での遮蔽変化をソフトに
		bl += step(falloff, depthDifference) * normDiff * (1.0 - smoothstep(falloff, strength, depthDifference));
	}

	// output the result
	// AO項の計算。遮蔽されているほどblが大きいので、暗くなる。
	float ao = 1.0 - totStrength * bl * invSamples;
	Output.RGBColor = float4(ao, 0.0f, 0.0f, 1.0f);

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
	    VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader  = compile ps_3_0 RenderScenePS0();
    }
}