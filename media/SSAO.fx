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


float totStrength = 1.38;
float strength = 0.07;
float offset = 18.0;
float falloff = 0.000002;
float rad = 0.006;

#define SAMPLES 16 // 10 is good
const float invSamples = 1.0/16.0;

sampler rnm : register(s0);
sampler normalMap : register(s1);

// �P�ʋ����̃����_���ȃx�N�g��
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
//�o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[����s�N�Z���V�F�[�_�[�֓n�����߂̍\����
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //���_�̍��W
   float4 Col   : COLOR0;       //���_�J���[
   float4 depth : COLOR1 ;
   float3 normal : COLOR2 ;
//   float2 Tex   : TEXCOORD0;    //�e�N�Z�����W
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
//   Out.Tex    = Tex;
   
   //�Ɩ����t�����ɂ���B(���ӂP)
   float3 L = -normalize( m_LightDir.xyz );
   
   //�@���x�N�g���B
   float3 N = normalize( Normal.xyz );

   //�Ɩ��Ɩ@������dot�֐��ɂ��A���ς��v�Z����B�܂�max�֐��ɂ�蒸�_�J���[�̍ŏ��l�������ɗ}����悤�ɐݒ肷��B(���ӂQ)
   Out.Col = max( m_Ambient, dot(N, L) );
   Out.depth = Out.Pos;
   
   float4 normal = mul( Normal, m_WVP );
   Out.normal = normalize(normal);
   return Out;
}

//****************************************************************
//�s�N�Z���V�F�[�_�[
//****************************************************************
float4 PS( VS_OUTPUT In ): COLOR0
{  
   float4 Out = (float4)0;
   
   //�F����Col1�Ɋi�[����
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
// �s�N�Z���V�F�[�_ ���C�� http://www.gamerendering.com/2009/01/14/ssao/ �̈ڐA(���C��)
//--------------------------------------------------------------------------------------
SSPS_OUTPUT RenderScenePS0(
	float2 uv		: TEXCOORD0
)
{
    SSPS_OUTPUT Output;

	// grab a normal for reflecting the sample rays later on
	// ��ŃT���v�������𔽎˂����邽�߂̖@����
	// rnm :�����_���m�[�}���}�b�v�̃T���v���B�����_���m�[�}���}�b�v�̓T���v�����O�������_���ɂ��邽�߂Ɏg���B
	// offset : �����_���m�[�}���}�b�v���`��T�C�Y��菬�������Ƃɂ���ē����s�N�Z�����T���v�������̂�h���B
	float3 fres = normalize((tex2D(rnm, uv*offset).xyz*2.0) - 1.0f);

	// RGB�ɖ@�����Aa��0�`1�̃f�v�X�������Ă���
	float4 currentPixelSample = tex2D(normalMap, uv);

	float currentPixelDepth = currentPixelSample.a;

	// get the normal of current fragment
	// ���݃t���O�����g�̖@��
	float3 norm = currentPixelSample.xyz * 2.0f - 1.0f;


	float bl = 0.0;
	// adjust for the depth ( not shure if this is good..)
	// �T���v�����O���a�B��O�قǎ�������鋗����傫������(�p�[�X�̕␳)
	float radD = rad / currentPixelDepth;
	//float radD = rad;

	float3 ray, occNorm;
	float2 se;
	float occluderDepth, depthDifference, normDiff;

	for(int i=0; i<SAMPLES;++i)
	{
		// get a vector (randomized inside of a sphere with radius 1.0) from a texture and reflect it
		// �e�N�X�`������擾����(���a1.0�̋����Ɏ��܂��������_����)�x�N�g���ɔ��˂�����
		// �Ȃ��fres�����̂܂�ray�Ƃ��Ďg��Ȃ��񂾂�B�������o���������������炩�ȁB(fres�͐��K������Ă�)
		// radD�͐[�x�l�̕΂�̕␳�B
		ray = radD * reflect(pSphere[i], fres);

		// if the ray is outside the hemisphere then change direction
		// �����������̊O(����)�Ȃ甽�]����
		// ���΂���dot�����l��Ԃ��̂�sign��-1��Ԃ�
		se = uv + sign(dot(ray,norm)) * ray * float2(1.0f, -1.0f);

		// get the depth of the occluder fragment
		// �Օ�����t���O�����g���擾
		float4 occluderFragment = tex2D(normalMap, se.xy);

		// get the normal of the occluder fragment
		// �Օ�����t���O�����g�̖@�����擾
		occNorm = occluderFragment.xyz * 2.0f - 1.0f;

		// if depthDifference is negative = occluder is behind current fragment
		// ���F�Օ��t���O�����g�͌��݂̃t���O�����g���O ���Օ�����Ă�
		// ���F�Օ��t���O�����g�͌��݂̃t���O�����g��艜 ���Օ�����ĂȂ�
		depthDifference = currentPixelDepth - occluderFragment.a;

		// calculate the difference between the normals as a weight
		// �Օ��t���O�����g�ƌ��݃t���O�����g�ł̖@���̊p�x�̍����v�Z
		// �������Ȃ�1�A���΂Ȃ�0
		// �������������Ă�����Օ�����Ă��Ȃ��͂��A�p�x������������ΎՕ�����Ă���͂��A�Ƃ�������
		normDiff = (1.0 - dot(normalize(occNorm), normalize(norm)));

		// the falloff equation, starts at falloff and is kind of 1/x^2 falling
		// �������B1/x^2 �Ȋ����Ō�������B
		// step : step(edge, x) 0.0 if x < edge, else 1.0
		//        depthDifference �� falloff ��菬������� 0�A�����łȂ���� 1�B
		//        ��������x�ȏ��O�ɂ���ΎՕ�
		// smoothstep : depthDifference �� falloff �ȉ��Ȃ� 0�Astrength �ȏ�Ȃ� 1�A�����łȂ���ΊԂ���
		//        ��1.0-smoothstep(...) �ŁA
		//          �Օ��t���O�����g�����݃t���O�����g��菭���O�̏ꍇ�A�\�t�g�ɈÂ�����B
		//          �܂��A�Օ��t���O�����g����O�߂���ꍇ�͎Օ��ƌ��Ȃ��Ȃ��B
		//          (falloff��step�̕��őO��֌W�ɂ��Օ�����ɂ��g���Ă���̂ŁAsmoothstep�̕��ł͕�Ԃ̊J�n�ʒu�Ƃ������x�̈Ӗ�)
		//
		//    �O��֌W�ł̎Օ��L��         �p�x�ɂ��Օ�           �O��֌W�ł̎Օ��ω����\�t�g��
		bl += step(falloff, depthDifference) * normDiff * (1.0 - smoothstep(falloff, strength, depthDifference));
	}

	// output the result
	// AO���̌v�Z�B�Օ�����Ă���ق�bl���傫���̂ŁA�Â��Ȃ�B
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