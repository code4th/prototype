//--------------------------------------------------------------------------------------
// File: ssao.fx
//
// The effect file for the Screen Space Ambient Occlusion.
//--------------------------------------------------------------------------------------

//fxc ssao.fx /T fx_2_0 /Fo ssao.fxo

//���[���h�A�r���[�A�ˉe���W�ϊ��}�g���b�N�X
float4x4 m_WVP;

//�Ɩ��̕����x�N�g��
float4 m_LightDir;

//�����ɑ΂���}�e���A���B0.0f�`1.0f�̒l�������A1.0f�̂Ƃ��ł������Ȃ�B
float4 m_Ambient = 0.0f;
int m_iFlag;

//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[����s�N�Z���V�F�[�_�[�֓n�����߂̍\����
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //���_�̍��W
   float4 Col   : COLOR0;       //���_�J���[
   float4 depth : COLOR1 ;
   float3 normal : COLOR2 ;
   float2 Tex   : TEXCOORD0;    //�e�N�Z�����W
};
//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS( float4 Pos     : POSITION,   //���_�̍��W
              float4 Normal  : NORMAL,     //�@���x�N�g��
              float2 Tex     : TEXCOORD0 ) //�e�N�Z��
{
   VS_OUTPUT Out = (VS_OUTPUT)0;
   
   //���_�̍��W���s��ϊ�����
   Out.Pos    = mul( Pos, m_WVP );
   
   //�e�N�Z���͂��̂܂܊i�[����
   Out.Tex    = Tex;
   
   //�Ɩ����t�����ɂ���B(���ӂP)
   float3 L = -normalize( m_LightDir.xyz );
   
   //�@���x�N�g���B
   float3 N = normalize( Normal.xyz );

   //�Ɩ��Ɩ@������dot�֐��ɂ��A���ς��v�Z����B�܂�max�֐��ɂ�蒸�_�J���[�̍ŏ��l�������ɗ}����悤�ɐݒ肷��B(���ӂQ)
   Out.Col = max( m_Ambient, dot(N, L) );
   Out.depth = Out.Pos;
   
   Out.normal = Normal.xyz;
   Out.normal = normalize(Out.normal);
   return Out;
}

//****************************************************************
//�s�N�Z���V�F�[�_�[
//****************************************************************
sampler tex0 : register(s0) = sampler_state 
{ 
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
}; 

struct SP_OUTPUT
{
	float4 Color		: COLOR0;	// �J���[
	float4 NormalDepth  : COLOR1;	// �@���Ɛ[�x
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
// �s�N�Z���V�F�[�_ ���C�� http://www.gamerendering.com/2009/01/14/ssao/ �̈ڐA(���C��)
//--------------------------------------------------------------------------------------
SSPS_OUTPUT SSAO( float2 uv		: TEXCOORD0 )
{

    SSPS_OUTPUT Output;

	// ���݃t���O�����g�̃s�N�Z��
	float4 currentPixel = tex2D(normalMap, uv);

	// ���݃t���O�����g�̖@��
	float3 currentNormal = currentPixel.xyz * 2.0f - 1.0f;
	// ���݃t���O�����g�̐[�x
	float currentDepth = currentPixel.a;
	// ���݃t���O�����g�̍��W
	float3 currentPos = float3(uv.x, uv.y, currentDepth);

	// �T���v�����O���a�B��O�قǎ�������鋗����傫������(�p�[�X�̕␳)
	float radD = rad / currentDepth;

	float ao = 0.0;
	float4 gi = 0.0;
	for(int i=0; i<SAMPLES;++i)
	{
		float3 fres = tex2D(rayMap, float2(float(i)/16.f,0)).xyz*2.f-1.f;
		float3 ray = radD * fres;

		// �����������̊O(����)�Ȃ甽�]����
		float2 sample = currentPos + sign(dot(ray,currentNormal) )*ray;
		// �`�F�b�N����t���O�����g���擾
		float4 samplePixel = tex2D(normalMap, sample.xy);
		// �`�F�b�N����t���O�����g�̖@�����擾
		float3 sampleNormal = samplePixel.xyz * 2.0f - 1.0f;
		// �`�F�b�N����t���O�����g�̐F
		float4 sampleColor = tex2D(colorMap, sample.xy);
		// �`�F�b�N����t���O�����g�̍��W
		float3 samplePos = float3(sample.x, sample.y, samplePixel.a);


		// �[�x�l�̍�
		float dDepth = currentDepth - samplePixel.a;

		// �@���̍�
		float dNormal = (1.0 - dot(normalize(sampleNormal), normalize(currentNormal)));

		//    �O��֌W�ł̎Օ��L��         �p�x�ɂ��Օ�           �O��֌W�ł̎Օ��ω����\�t�g��
		ao += step(falloff, dDepth) * dNormal * (1.0 - smoothstep(falloff, strength, dDepth));
//		float invLen = 1.f/length(currentPos-samplePos);
		gi += ao * sampleColor;
	}

	// AO���̌v�Z�B�Օ�����Ă���ق�bl���傫���̂ŁA�Â��Ȃ�B
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