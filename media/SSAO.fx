//--------------------------------------------------------------------------------------
// File: ssao.fx
//
// The effect file for the Screen Space Ambient Occlusion.
//--------------------------------------------------------------------------------------

//fxc ssao.fx /T fx_2_0 /Fo ssao.fxo

//���[���h�A�r���[�A�ˉe���W�ϊ��}�g���b�N�X
float4x4 m_WVP;
float4x4 m_View;
float4x4 m_Proj;

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
   float4 Pos   : POSITION;     // ���_�̍��W
   float4 Col   : COLOR0;       // ���_�J���[
   float4 Depth : COLOR1;		// �[�x
   float3 Normal: COLOR2;		// �@��
   float2 Tex   : TEXCOORD0;    //�e�N�Z�����W
};
//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS( float4 Pos     : POSITION,   //���_�̍��W
              float4 Normal  : NORMAL,     //�@���x�N�g��
              float4 Tangent  : TANGENT,     //�@���x�N�g��
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

	Out.Depth = Out.Pos;   
	// normal in eye space

//	float3x3 gl_NormalMatrix   = m_WVP;
//	gl_NormalMatrix = transpose(gl_NormalMatrix); 

    // �@���Ɛڐ�
    // �̕ϊ�
    float3 wNormal = mul( Normal, m_WVP );
    float3 wTangent = mul( Tangent, m_WVP );

    // �]�@�����쐬���邽�߂̊O��
    float3 wBinorm = cross( wTangent, wNormal );

    // �ŏ��̎�@�̓f�B�t���[�Y dot3 �o���v�}�b�v
    // ���C�g �x�N�g����ڋ�Ԃɕϊ�����   
    float3x3 gl_NormalMatrix = float3x3(wTangent, wBinorm, wNormal);
    gl_NormalMatrix = transpose(gl_NormalMatrix);

	Out.Normal= normalize( mul( Normal.xyz, gl_NormalMatrix )).xyz;

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
	float4 Position		: COLOR2;	// ���W
};
SP_OUTPUT PS( VS_OUTPUT In )
{  
	SP_OUTPUT Out = (SP_OUTPUT)0;
   
	Out.Color = In.Col * tex2D( tex0, In.Tex );

	Out.NormalDepth.rgb = (normalize(In.Normal.xyz)+1.0f)*0.5f;
	Out.NormalDepth.a = In.Depth.z/In.Depth.w;

	Out.Position = Out.NormalDepth;
	Out.Position.z = 0;

	return Out;
}


sampler rayMap : register(s0) = sampler_state 
{ 
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
}; 
sampler normalMap : register(s1) = sampler_state
{
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
};
sampler colorMap : register(s2) = sampler_state
{
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler posMap : register(s3) = sampler_state
{
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
};
/*
uniform float totStrength = 1.38;
uniform float strength = 0.07;
uniform float offset = 18.0;
uniform float falloff = 0.000002;
uniform float rad = 0.006;
*/

float falloff = 0.000002;
float totStrengthAO = 1.38;
float totStrengthGI = 0.60;
float strengthAO = 0.07;
float strengthGI = 0.07;
float rad = 0.006;

#define SAMPLES 16 // 10 is good
float invSamples = 1.0f/16.0f;


//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_ ���C�� http://www.gamerendering.com/2009/01/14/ssao/ �̈ڐA(���C��)
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
   float2 vPos   : VPOS;     // 
   float2 uv   : TEXCOORD0;    //�e�N�Z�����W
};


float4 SSAOPS( PS_INPUT In ) :COLOR0
{
	// ���݃t���O�����g�̃s�N�Z��
	float4 currentPixel = tex2D(normalMap, In.uv);

//	return float4(currentPixel.xyz,1);

	// ���݃t���O�����g�̖@��
	float3 currentNormal = currentPixel.xyz * 2.0f - 1.0f;
	// ���݃t���O�����g�̐[�x
	float currentDepth = currentPixel.a;
	// ���݃t���O�����g�̍��W
	float3 currentPos = float3(In.uv, currentDepth);
//	float3 currentPos = tex2D(posMap, In.uv);
	// ���݃t���O�����g�̃J���[
	float4 curentColor = tex2D(colorMap, In.uv);

	// �T���v�����O���a�B��O�قǎ�������鋗����傫������(�p�[�X�̕␳)
	float radD = rad / currentDepth;

	float ao = 0.f;
	float4 gi = 0.f;
	for(int i=0; i<SAMPLES;++i)
	{
		float3 fres = tex2D(rayMap, float2(float(i)/float(SAMPLES),0)).xyz*2.f-1.f;
		float3 ray = radD * fres;

		// �����������̊O(����)�Ȃ甽�]����
		float3 samplePos = currentPos + sign( dot(ray,currentNormal) )*ray;
		// �`�F�b�N����t���O�����g���擾
		float4 samplePixel = tex2D(normalMap, samplePos.xy);
//		float4 samplePixel = tex2D(normalMap, (mul(samplePos,m_View)+1.f).xy*0.5f);
		// �`�F�b�N����t���O�����g�̖@�����擾
		float3 sampleNormal = samplePixel.xyz * 2.0f - 1.0f;
		// �`�F�b�N����t���O�����g�̐F
		float4 sampleColor = tex2D(colorMap, samplePos.xy);
		// �`�F�b�N����t���O�����g�̍��W
		float sampleDepth = samplePixel.a;


		// �[�x�l�̍�

		float dDepth = currentDepth - sampleDepth;
		float dDepth2 = samplePos.z - sampleDepth;

		// �@���̍�
		float dNormal = 1.0f - dot(normalize(sampleNormal), normalize(currentNormal));

		//    �O��֌W�ł̎Օ��L��         �p�x�ɂ��Օ�           �O��֌W�ł̎Օ��ω����\�t�g��
//		ao += step(falloff, dDepth) * dNormal * (1.0 - smoothstep(falloff, strengthAO, dDepth));
		ao += smoothstep(falloff, strengthAO, dDepth2);
		gi += step(falloff, dDepth) * dNormal * (1.0 - smoothstep(falloff, strengthGI, dDepth)) * sampleColor;
	}

	ao = 1.0f - totStrengthAO * ao * invSamples;
	gi = totStrengthGI * gi * invSamples;

	return float4(ao,0,0,1);
//	return curentColor*ao+gi;

}

void DecodeDepthNormal(in float4 _in, out float _depth, out float3 _normal)
{
	// ���݃t���O�����g�̖@��
	_normal = _in.xyz * 2.0f - 1.0f;
	// ���݃t���O�����g�̐[�x
	_depth = _in.w;

}

float4 SSGI(      half4 uv : TEXCOORD0 ): COLOR0
{
    half radius = 0.2;
    half intensity = 0.01;	// ���x

    // Kernel definition
    const half3 kernel[32] = {
    half3(-0.556641,-0.037109,-0.654297),
    half3(0.173828,0.111328,0.064453),
    half3(0.001953,0.082031,-0.060547),
    half3(0.220703,-0.359375,-0.062500),
    half3(0.242188,0.126953,-0.250000),
    half3(0.070313,-0.025391,0.148438),
    half3(-0.078125,0.013672,-0.314453),
    half3(0.117188,-0.140625,-0.199219),
    half3(-0.251953,-0.558594,0.082031),
    half3(0.308594,0.193359,0.324219),
    half3(0.173828,-0.140625,0.031250),
    half3(0.179688,-0.044922,0.046875),
    half3(-0.146484,-0.201172,-0.029297),
    half3(-0.300781,0.234375,0.539063),
    half3(0.228516,0.154297,-0.119141),
    half3(-0.119141,-0.003906,-0.066406),
    half3(-0.218750,0.214844,-0.250000),
    half3(0.113281,-0.091797,0.212891),
    half3(0.105469,-0.039063,-0.019531),
    half3(-0.705078,-0.060547,0.023438),
    half3(0.021484,0.326172,0.115234),
    half3(0.353516,0.208984,-0.294922),
    half3(-0.029297,-0.259766,0.089844),
    half3(-0.240234,0.146484,-0.068359),
    half3(-0.296875,0.410156,-0.291016),
    half3(0.078125,0.113281,-0.126953),
    half3(-0.152344,-0.019531,0.142578),
    half3(-0.214844,-0.175781,0.191406),
    half3(0.134766,0.414063,-0.707031),
    half3(0.291016,-0.833984,-0.183594),
    half3(-0.058594,-0.111328,0.457031),
    half3(-0.115234,-0.287109,-0.259766),
    };

    half4 screenTC = uv;

    // Compute dithering vector �덷�g�U�I��
    half3 normalDither = normalize(tex2D(rayMap, screenTC.wz).xyz * 2.h - 1.h);

    // Get view space normal and depth
    float4 encodedDepthNormal = tex2D(normalMap, screenTC.xy);
    float depth;
    float3 normal_vs;
    DecodeDepthNormal(encodedDepthNormal, depth, normal_vs);
    //depth *= _ProjectionParams.z;

    // Init the occlusion
    half3 colorBleeding = 0;
    for (int i = 0; i < SAMPLES; i++) {
    half3 sample;

    // Construct the sample
    sample = reflect(kernel[i] * radius, normalDither);
    sample = sample * (dot(sample, normal_vs) < 0 ? -1:1);

    // Find the offset
    half gamma = 1.h / (1.h + sample.z);
    half2 centerPos = (screenTC.xy * 2) - half2(1, 1);
    half2 samplePos = (centerPos + sample.xy) * (gamma * 0.5h) + 0.5h;

    // Get view space normal and depth
    float4 sampleEncodedDepthNormal;
    float tapDepth = 0.0h;
    float3 tapNormal_vs;
    sampleEncodedDepthNormal = tex2D(normalMap, samplePos);
    DecodeDepthNormal(sampleEncodedDepthNormal, tapDepth, tapNormal_vs);
    //tapDepth *= _ProjectionParams.z;

    // Get radiance.
    half3 tapRadiance = tex2D(colorMap, samplePos);

    // Compute the direction vector between the point and the bleeder
    half alpha = gamma * tapDepth;
    half3 D = half3(centerPos, 1.h) * (alpha - depth) + sample * alpha;
    const float minDist = 0.0005;
    half r = max(minDist, length(D));
    D = normalize(D);

    // Compute attenuation
    half atten =
        //pow(minDist / r, 2)
        //min(centerDepth * pow(0.05 / r, 2), 1)
        min(pow(0.05 * depth / r, 2), 1)
        //1
        ;

    float factor = 400
        // Visibility
        * ((depth * (1 + sample.z) - tapDepth) > 0)
        // Reflector lambertian term
        * max(0, -dot(D, tapNormal_vs))
        // Incident radiance projection term
        * dot(D, normal_vs) 
        // Attenuation term
        * atten
        ;

    half3 radiance = factor * tapRadiance;
    colorBleeding += radiance;
    }

    // Normalize result
    colorBleeding *= intensity / (half)SAMPLES;
    return half4(max(0,colorBleeding), depth);
}
/*
float4 SSDO (half4 uv : TEXCOORD0) : COLOR0
{
    half4 scene = tex2D(colorMap, uv);
    half ao = SSAOPS(uv);
    float3 gi = SSGI(uv);

	return float4(gi, 1.0);

//    return float4(scene.rgb *ao + gi, 1.0);
}
*/
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
        PixelShader  = compile ps_3_0 SSAOPS();
//        PixelShader  = compile ps_3_0 SSDO();
    }
}