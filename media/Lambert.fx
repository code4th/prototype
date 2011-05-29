
//****************************************************************
// �����o�[�g�g�U�Ɩ�
//****************************************************************

//�O������ݒ肷��ϐ��S

//���[���h�A�r���[�A�ˉe���W�ϊ��}�g���b�N�X
float4x4 m_WVP;

//�Ɩ��̕����x�N�g��
float4 m_LightDir;

//�����ɑ΂���}�e���A���B0.0f�`1.0f�̒l�������A1.0f�̂Ƃ��ł������Ȃ�B
float4 m_Ambient = 0.0f;

//�I�u�W�F�N�g�̃e�N�X�`���[�B�O�������IDirect3DDevice9::SetTexture�֐��Őݒ肷��B
sampler tex0 : register(s0);

//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[����s�N�Z���V�F�[�_�[�֓n�����߂̍\����
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //���_�̍��W
   float4 Col   : COLOR0;       //���_�J���[
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
       
   return Out;
}

//****************************************************************
//�s�N�Z���V�F�[�_�[
//****************************************************************
float4 PS( VS_OUTPUT In ) : COLOR0
{  
   float4 Out = (float4)0;
   
   //�F����Col1�Ɋi�[����
   Out = In.Col * tex2D( tex0, In.Tex );
      
   return Out;
}

technique TShader
{
   pass P0
   {
      //vs_1_1�����ps_1_1�̓o�[�e�b�N�X�V�F�[�_�[�A�s�N�Z���V�F�[�_�[���ꂼ��Ŏg�p����o�[�W�����B
      //�Ⴂ�ق������s�\�Ȋ��������Ȃ�̂łł��邾���Ⴍ�ݒ肷��Ƃ悢�B
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();   
   }     
}