
//****************************************************************
// ランバート拡散照明
//****************************************************************

//外部から設定する変数郡

//ワールド、ビュー、射影座標変換マトリックス
float4x4 m_WVP;

//照明の方向ベクトル
float4 m_LightDir;

//環境光に対するマテリアル。0.0f～1.0fの値を持ち、1.0fのとき最も強くなる。
float4 m_Ambient = 0.0f;

//オブジェクトのテクスチャー。外部からはIDirect3DDevice9::SetTexture関数で設定する。
sampler tex0 : register(s0);

//****************************************************************
//バーテックスシェーダー
//****************************************************************
//バーテックスシェーダーからピクセルシェーダーへ渡すための構造体
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //頂点の座標
   float4 Col   : COLOR0;       //頂点カラー
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
       
   return Out;
}

//****************************************************************
//ピクセルシェーダー
//****************************************************************
float4 PS( VS_OUTPUT In ) : COLOR0
{  
   float4 Out = (float4)0;
   
   //色情報をCol1に格納する
   Out = In.Col * tex2D( tex0, In.Tex );
      
   return Out;
}

technique TShader
{
   pass P0
   {
      //vs_1_1およびps_1_1はバーテックスシェーダー、ピクセルシェーダーそれぞれで使用するバージョン。
      //低いほうが実行可能な環境が多くなるのでできるだけ低く設定するとよい。
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();   
   }     
}