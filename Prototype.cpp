#include "Prototype.h"
#include "kickflip/kickflipHashString.h"
#include "kickflip/kickflipEmbedded.h"
#include "kickflip/kickflipDebugFont.h"
#include "kickflip/kickflipThread.h"

Prototype theApp;

using namespace kickflip;
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
		_T("media/Lambert.fx"),
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


	// ビュー変換・射影変換
	D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian(45), 640.0f/480.0f, 1.0f, 10000.0f);


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

void Prototype::UpdateFrame()
{
	DebugPrint(0,0,"time:(frame:%f realtime:%d)",Time::GetFrameTimeSecond(),Time::GetFrameTimeMicroSecond());
	DebugPrint(0,1,"deltaSecond     :(frame:%f,realtime:%f)",Time::GetFrameDeltaTimeSecond(),Time::GetRealDeltaTimeSecond());
	DebugPrint(0,2,"deltaMicroSecond:(frame:%d)",Time::GetFrameDeltaTimeMicroSecond());
	DebugPrint(0,3,"deltaMicroSecond:(realtime:%d)",Time::GetRealDeltaTimeMicroSecond());
	DebugPrint(0,4,"fps:%f(ave:%.1f)\n",Time::GetFPS(),Time::GetFPSAve());

	f+=static_cast<float>(Time::GetFrameDeltaTimeSecond());
	static int iSleep = 1;
	DebugPrint(0,11,"sleep:%d",iSleep);
	if(true == GamePad(0).IsPressed(InputDevice::GamePad::A))
	{
		m_rpActionController->ChangeAction(_H("Punch"));
	}
	if(true == GamePad(0).IsPressed(InputDevice::GamePad::B))
	{
		m_rpActionController->ChangeAction(_H("Kick"));
	}
	if(true == GamePad(0).IsOn(InputDevice::GamePad::L1))
	{
		--iSleep;
	}
	if(true == GamePad(0).IsOn(InputDevice::GamePad::R1))
	{
		++iSleep;
	}
	if(0>iSleep)iSleep=0;
	Sleep(iSleep);
	m_rpActionController->Update();

	GetInputDevice()->DebugPrintGamePad(0,5);

	// エフェクト内のワールドビュー射影変換行列を設定
	D3DXMATRIX mat;
	D3DXMatrixLookAtLH( &View, &D3DXVECTOR3(30*sin(f),20,-30*cos(f)), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
	D3DXMatrixIdentity( &mat );
	mat = mat * View * Proj;
	//   pEffect->SetMatrix( "matWorldViewProj", &mat );
	pEffect->SetMatrix( "m_WVP", &mat );
	/*
	//fxファイル内で宣言している変数のハンドルを取得する
	m_pTechnique = m_pEffect->GetTechniqueByName( "TShader" );
	m_pWVP = m_pEffect->GetParameterByName( NULL, "m_WVP" );
	m_pLightDir = m_pEffect->GetParameterByName( NULL, "m_LightDir" );
	m_pAmbient = m_pEffect->GetParameterByName( NULL, "m_Ambient" );

	m_pEffect->SetTechnique( m_pTechnique );
	*/

	// 描画開始
	pEffect->SetTechnique( "TShader" );
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
	pEffect->End();


}