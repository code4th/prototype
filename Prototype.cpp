#include "Prototype.h"
#include "kickflip/kickflipHashString.h"
#include "kickflip/kickflipEmbedded.h"
#include "kickflip/kickflipDebugFont.h"
#include "kickflip/kickflipThread.h"

Prototype theApp;

using namespace kickflip;

void Prototype::InitSetting()
{
	SetScreenHeight(256);
}
static void WINAPI make_noise_radius(D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, const D3DXVECTOR2* pTexelSize, void* data) {
  static unsigned int a = 0;
  float r = 0.1f * (float)rand() / (float)RAND_MAX;
  float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
  float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
  float sp = sqrt(1.0f - cp * cp);
  float ct = cos(t), st = sin(t);


  pOut->x = r * sp * ct;
  pOut->y = r * sp * st;
  pOut->z = r * cp;
  pOut->w = 0;
}
void Prototype::ExecOnceBeforeUpdate()
{

	m_rpResouceManager = new ResourceManager();
/*
	for(auto i=0;i<10;i++)
		m_rpThreadList.push_back(Thread::Create(new Test("Thread1",10,10)));

	for(auto ite = m_rpThreadList.begin(); m_rpThreadList.end() != ite; ite++)
	{
		(*ite)->Resume();
	}
	*/
	hashString hash = HashString("teset");

	hashString t1 = HashString("a");
	hashString t2 = HashString("b");
	hashString t3 = HashString("c");

	hashString dhash1 = HashString("standardShader.fx");
	hashString shash1 = HashString("standardShader.fx");
	hashString dhash2 = HashString("standardShader1.fx");
	hashString shash2 = HashString("standardShader1.fx");
	hashString dhash3 = HashString("standardShades.fx");
	hashString shash3 = HashString("standardShades.fx");




	GetGraphicShader()->Load(HashString("Test1.fx"),1);
	GetGraphicShader()->Load(HashString("Test2.fx"),1);
	GetGraphicShader()->Load(HashString("Test1.fx"),2);
	GetGraphicShader()->Load(HashString("Test2.fx"),2);

	GetGraphicShader()->Load(HashString("Test1.fx"),1);
	GetGraphicShader()->Load(HashString("Test2.fx"),1);
	GetGraphicShader()->Load(HashString("Test1.fx"),2);
	GetGraphicShader()->Load(HashString("Test2.fx"),2);

	// エフェクトの読み込み
	LPD3DXBUFFER pErr = NULL;

	HRESULT hr = D3DXCreateEffectFromFile(
		GetGraphicDevice()->GetDevice(),
//		_T("media/Lambert.fx"),
		_T("media/SSAO.fx"),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&pEffect,
		&pErr
		);
	if( FAILED( hr ) )
	{
		if( pErr != NULL )
		{	// コンパイルエラー出力
			const char *err = (char*)pErr->GetBufferPointer();
			DebugTrace("shaderERR:%s\n", err );

			pErr->Release();
		}
	}
	GetGraphicDevice()->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	GetGraphicDevice()->GetDevice()->CreateTexture(GetScreenWidth(), GetScreenHeight(), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texture0, 0);
	texture0->GetSurfaceLevel(0, &surface0);

	GetGraphicDevice()->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GetGraphicDevice()->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GetGraphicDevice()->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	GetGraphicDevice()->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	GetGraphicDevice()->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//Create Noise
	GetGraphicDevice()->GetDevice()->CreateTexture(64, 64, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &tex_noise_ssao, 0);
	D3DXFillTexture(tex_noise_ssao, make_noise_radius, 0);


	// ビュー変換・射影変換
	D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian(45), static_cast<float>(GetScreenWidth())/static_cast<float>(GetScreenHeight()), 1.0f, 10000.0f);


	class Idol :public Action
	{
	public:
		Idol(hashString kHash)
			: Action(kHash)
		{
		}
		bool Update()
		{
			return Action::Update();
		}
	};
	class Punch :public Action
	{
	public:
		Punch(hashString kHash)
			: Action(kHash)
		{
		}
		bool Update()
		{
			Action::Update();
			DebugPrint(10,10,"Punch");
			float fTime = (float)(Time::GetFrameTimeSecond()-m_fEnterTime);
			if(0.05f<=fTime) return false;

			return true;
		}
	};
	class Kick :public Action
	{
	public:
		Kick(hashString kHash)
			: Action(kHash)
		{
		}
		bool Update()
		{
			Action::Update();
			DebugPrint(10,10,"Kick");
			float fTime = (float)(Time::GetFrameTimeSecond()-m_fEnterTime);
			if(1.0f<=fTime) return false;

			return true;
		}
	};

	m_rpActionController = new ActionController();

	m_rpActionController->Regist( new Idol(_H("Idol")) );
	m_rpActionController->Regist( new Punch(_H("Punch")) );
	m_rpActionController->Regist( new Kick(_H("Kick")) );
	
	m_rpActionController->ChangeAction(_H("Punch"));

	// Xファイルオブジェクト読み込み
	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/Head_Big_Ears.x")));
	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/Head_Sad.x")));
	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/LandShark.x")));
	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/wall_with_pillars.x")));

/*
	while(1)
	{
		bool bIsNext = true;
		for(auto ite = m_kMeshObjectList.begin(); m_kMeshObjectList.end()!=ite; ite++)
		{
			if(false == (*ite)->IsComplete())
			{
				bIsNext = false;
			}
		}
		if(true == bIsNext) break;
	}
*/
}
void Prototype::BeforePresent()
{
	Framework::Get().GetGraphicDevice()->Lock();
}
void Prototype::AfterPresent()
{
	Framework::Get().GetGraphicDevice()->Unlock();
}

struct d3dverts {
  float x, y, z, w;
  float u, v;
  enum { fvf = D3DFVF_XYZRHW | D3DFVF_TEX1 };
};


void Prototype::UpdateFrame()
{
	DebugPrint(0,0,"time:(frame:%f realtime:%d)",Time::GetFrameTimeSecond(),Time::GetFrameTimeMicroSecond());
	DebugPrint(0,1,"deltaSecond     :(frame:%f,realtime:%f)",Time::GetFrameDeltaTimeSecond(),Time::GetRealDeltaTimeSecond());
	DebugPrint(0,2,"deltaMicroSecond:(frame:%d)",Time::GetFrameDeltaTimeMicroSecond());
	DebugPrint(0,3,"deltaMicroSecond:(realtime:%d)",Time::GetRealDeltaTimeMicroSecond());
	DebugPrint(0,4,"fps:%f(ave:%.1f)\n",Time::GetFPS(),Time::GetFPSAve());

	f+=static_cast<float>(Time::GetFrameDeltaTimeSecond());
	l=sin(f)*50.f+800.f;

	if(true == GamePad(0).IsPressed(InputDevice::GamePad::A))
	{
		m_rpActionController->ChangeAction(_H("Punch"));
	}
	if(true == GamePad(0).IsPressed(InputDevice::GamePad::B))
	{
		m_rpActionController->ChangeAction(_H("Kick"));
	}

	m_rpActionController->Update();

	GetInputDevice()->DebugPrintGamePad(0,5);

	// エフェクト内のワールドビュー射影変換行列を設定
	D3DXMATRIX mat;
	D3DXMatrixLookAtLH( &View, &D3DXVECTOR3(l*sin(f),20,-l*cos(f)), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
	D3DXMatrixIdentity( &mat );
	mat = mat * View * Proj;

	pEffect->SetMatrix( "m_WVP", &mat );
	pEffect->SetVector( "m_LightDir", &D3DXVECTOR4(1,1,1,0) );
	pEffect->SetVector( "m_Ambient" , &D3DXVECTOR4(1,0,0,0));

	LPDIRECT3DDEVICE9 d3ddevice = Framework::Get().GetGraphicDevice()->GetDevice();

	d3ddevice->SetRenderTarget(0, surface0);
    d3ddevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	// 描画開始
	pEffect->SetTechnique( "RenderScene0" );
	UINT numPass;
	pEffect->Begin( &numPass, 0 );

	pEffect->BeginPass(0);
	for(auto ite = m_kMeshObjectList.begin(); m_kMeshObjectList.end()!=ite; ite++)
	{
/*
		if((*ite)->IsLoadFinish())
		{
			(*ite)->Finish();
		}
*/
		(*ite)->Draw();
	}
	pEffect->EndPass();

	d3ddevice->SetRenderTarget(0, backbuffer);
    d3ddevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    d3ddevice->SetTexture(0, tex_noise_ssao );
    d3ddevice->SetTexture(1, texture0 );

	pEffect->BeginPass(1);
	d3ddevice->SetFVF( d3dverts::fvf  );
	const d3dverts quadverts[4] = {
		{ -0.5f+0.f,				-0.5f+      0.f, 0.0f, 1.0f, 0.0f, 0.0f},
		{ -0.5f+GetScreenWidth(),	-0.5f+      0.f, 0.0f, 1.0f, 1.0f, 0.0f},
		{ -0.5f+0.f,				-0.5f+GetScreenHeight(), 0.0f, 1.0f, 0.0f, 1.0f},
		{ -0.5f+GetScreenWidth(),	-0.5f+GetScreenHeight(), 0.0f, 1.0f, 1.0f, 1.0f},
	};

    d3ddevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quadverts, sizeof(d3dverts));

	pEffect->EndPass();

	pEffect->End();




}