#include "Prototype.h"
#include "kickflip/kickflipHashString.h"
#include "kickflip/kickflipEmbedded.h"
#include "kickflip/kickflipDebugFont.h"
#include "kickflip/kickflipThread.h"

Prototype theApp;

using namespace kickflip;
void Prototype::ExecOnceBeforeUpdate()
{
	m_rpThreadList.push_back(Thread::Create(new Test("Thread1",10,10)));
	m_rpThreadList.push_back(Thread::Create(new Test("Thread2",10,11)));
	m_rpThreadList.push_back(Thread::Create(new Test("Thread3",10,12)));
	m_rpThreadList.push_back(Thread::Create(new Test("Thread4",10,13)));

	for(auto ite = m_rpThreadList.begin(); m_rpThreadList.end() != ite; ite++)
	{
		(*ite)->Resume();
	}

	hash32 hash = HashString("teset");

	hash32 t1 = HashString("a");
	hash32 t2 = HashString("b");
	hash32 t3 = HashString("c");

	hash32 dhash1 = HashString("standardShader.fx");
	hash32 shash1 = HashString("standardShader.fx");
	hash32 dhash2 = HashString("standardShader1.fx");
	hash32 shash2 = HashString("standardShader1.fx");
	hash32 dhash3 = HashString("standardShades.fx");
	hash32 shash3 = HashString("standardShades.fx");


	// Xファイルオブジェクト読み込み
	D3DXLoadMeshFromX( _T("media/wall_with_pillars.x"), D3DXMESH_MANAGED, GetGraphicDevice(), NULL, &pMatBuf, NULL, &dwMatNum, &pMesh );
	pMatAry = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();

	GetGraphicShader().Load(HashString("Test1.fx"),1);
	GetGraphicShader().Load(HashString("Test2.fx"),1);
	GetGraphicShader().Load(HashString("Test1.fx"),2);
	GetGraphicShader().Load("Test2.fx",2);

	GetGraphicShader().Load("Test1.fx",1);
	GetGraphicShader().Load("Test2.fx",1);
	GetGraphicShader().Load("Test1.fx",2);
	GetGraphicShader().Load("Test2.fx",2);

	// エフェクトの読み込み
	LPD3DXBUFFER pErr = NULL;

	HRESULT hr = D3DXCreateEffectFromFile(
		GetGraphicDevice(),
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
			DebugOutput("shaderERR:%s\n", err );

			pErr->Release();
		}
	}


	// ビュー変換・射影変換
	D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian(45), 640.0f/480.0f, 1.0f, 10000.0f);



}

void Prototype::UpdateFrame()
{
	DebugPrint(0,0,"time:(frame:%f realtime:%d)",Time::GetFrameTimeSecond(),Time::GetFrameTimeMicroSecond());
	DebugPrint(0,1,"deltaSecond     :(frame:%f,realtime:%f)",Time::GetFrameDeltaTimeSecond(),Time::GetRealDeltaTimeSecond());
	DebugPrint(0,2,"deltaMicroSecond:(frame:%d)",Time::GetFrameDeltaTimeMicroSecond());
	DebugPrint(0,3,"deltaMicroSecond:(realtime:%d)",Time::GetRealDeltaTimeMicroSecond());
	DebugPrint(0,4,"fps:%f(ave:%f)\n",Time::GetFPS(),Time::GetFPSAve());

	f+=static_cast<float>(Time::GetFrameDeltaTimeSecond());

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
/*
	for(DWORD i=0; i<dwMatNum; i++)
	{
	GetGraphicDevice()->SetMaterial( &(pMatAry[i].MatD3D) );
	pMesh->DrawSubset(i);
	}
*/
	pEffect->EndPass();
	pEffect->End();


}