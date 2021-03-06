#include <msgpack.hpp>
#include "Prototype.h"
#include "kickflip/HashString.h"
#include "kickflip/Embedded.h"
#include "kickflip/Debug/DebugFont.h"
#include "kickflip/Thread.h"
#include "kickflip/Geometry.h"
#include "nucl/Random.h"



Prototype theApp;

using namespace kickflip;

void Prototype::InitSetting()
{
	SetScreenHeight(640);
	nc::Random rnd;
	rnd.setSeed(100);
	int uZ= rnd.rand();

	Geometry::Line l;
	l.p;
	l.v;


}
static void WINAPI makeRayMap(D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, const D3DXVECTOR2* pTexelSize, void* data)
{
	float r = 1.0f * (float)rand() / (float)RAND_MAX;
	float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
	float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
	float sp = sqrt(1.0f - cp * cp);
	float ct = cos(t), st = sin(t);

	pOut->x = ((r * sp * ct)+1.f)*0.5f;
	pOut->y = ((r * sp * st)+1.f)*0.5f;
	pOut->z = ((r * cp)+1.f)*0.5f;
	pOut->w = 0;
/*
	static int i=0;

	D3DXVECTOR3 vec;
	switch(i%16)
	{
	case 0:		vec = D3DXVECTOR3(0.53812504f, 0.18565957f, -0.43192f); break;
	case 1:		vec = D3DXVECTOR3(0.13790712f, 0.24864247f, 0.44301823f); break;
	case 2:		vec = D3DXVECTOR3(0.33715037f, 0.56794053f, -0.005789503f); break;
	case 3:		vec = D3DXVECTOR3(-0.6999805f, -0.04511441f, -0.0019965635f); break;
	case 4:		vec = D3DXVECTOR3(0.06896307f, -0.15983082f, -0.85477847f); break;
	case 5:		vec = D3DXVECTOR3(0.056099437f, 0.006954967f, -0.1843352f); break;
	case 6:		vec = D3DXVECTOR3(-0.014653638f, 0.14027752f, 0.0762037f); break;
	case 7:		vec = D3DXVECTOR3(0.010019933f, -0.1924225f, -0.034443386f); break;
	case 8:		vec = D3DXVECTOR3(-0.35775623f, -0.5301969f, -0.43581226f); break;
	case 9:		vec = D3DXVECTOR3(-0.3169221f, 0.106360726f, 0.015860917f); break;
	case 10:	vec = D3DXVECTOR3(0.010350345f, -0.58698344f, 0.0046293875f); break;
	case 11:	vec = D3DXVECTOR3(-0.08972908f, -0.49408212f, 0.3287904f); break;
	case 12:	vec = D3DXVECTOR3(0.7119986f, -0.0154690035f, -0.09183723f); break;
	case 13:	vec = D3DXVECTOR3(-0.053382345f, 0.059675813f, -0.5411899f); break;
	case 14:	vec = D3DXVECTOR3(0.035267662f, -0.063188605f, 0.54602677f); break;
	case 15:	vec = D3DXVECTOR3(-0.47761092f, 0.2847911f, -0.0271716f); break;
	}

	pOut->x = (vec.x+1.f)*0.5f;
	pOut->y = (vec.y+1.f)*0.5f;
	pOut->z = (vec.z+1.f)*0.5f;

	pOut->w = 0.f;

	++i;
*/
}

void Prototype::ExecOnceBeforeUpdate()
{

	{

		msgpack::sbuffer buffer;
		msgpack::packer<msgpack::sbuffer> pk(&buffer);
		pk.pack(std::string("origin"));
		pk.pack(std::string("cmd:id"));
		pk.pack(int(10));
		pk.pack(std::string("cmd:position"));
		pk.pack(float(10.2));
		pk.pack(float(10.2));
		pk.pack(float(10.2));
		pk.pack(std::string("cmd:message"));
		pk.pack(float(128));
		pk.pack(float(256));
		pk.pack(std::string("hallo"));
		pk.pack(std::string("cmd:damage"));
		pk.pack(float(10000));

		// deserializes these objects using msgpack::unpacker.
		msgpack::unpacker pac;

		// feeds the buffer.
		pac.reserve_buffer(buffer.size());
		memcpy(pac.buffer(), buffer.data(), buffer.size());
		pac.buffer_consumed(buffer.size());

		// now starts streaming deserialization.
		msgpack::unpacked result;
		while(pac.next(&result)) {
			msgpack::object& obj = result.get();
			obj.convert();
		}
	}


	Network::Get();

	httpObject_ = Network::Get()->GetHttpObject();
	if(NULL != httpObject_)
		httpObject_->Request("pma026.pmc.co.jp","GET /~olbaid_lobby/login.php?host HTTP/1.0\r\n\r\n", false);

	/*
		http://ec2-175-41-224-156.ap-northeast-1.compute.amazonaws.com:7000/
	*/

	{
        std::vector<std::string> vec;
        vec.push_back("Hello");
        vec.push_back("MessagePack");
        // serialize it into simple buffer.
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, vec);

        // deserialize it.
        msgpack::unpacked msg;
        msgpack::unpack(&msg, sbuf.data(), sbuf.size());
 
        // print the deserialized object.
        msgpack::object obj = msg.get();
 
        // convert it into statically typed object.
        std::vector<std::string> rvec;
        obj.convert(&rvec);
	}


//	Network::Get()->RegistClient("ec2-175-41-224-156.ap-northeast-1.compute.amazonaws.com",7000);
	Network::Get()->RegistClient("ec2-175-41-211-129.ap-northeast-1.compute.amazonaws.com",7000);


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
			DebugTrace("%s\n", err );

			pErr->Release();
		}
	}
	// バックバッファを覚えとく
	GetGraphicDevice()->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	GetGraphicDevice()->GetDevice()->CreateTexture(GetScreenWidth(), GetScreenHeight(), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texColor, 0);
	texColor->GetSurfaceLevel(0, &surColor);
	GetGraphicDevice()->GetDevice()->CreateTexture(GetScreenWidth(), GetScreenHeight(), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texNormalDepth, 0);
	texNormalDepth->GetSurfaceLevel(0, &surNormalDepth);
	GetGraphicDevice()->GetDevice()->CreateTexture(GetScreenWidth(), GetScreenHeight(), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texPosition, 0);
	texPosition->GetSurfaceLevel(0, &surPosition);

	//Create Noise
//	GetGraphicDevice()->GetDevice()->CreateTexture(16, 16, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &rayMap, 0);
//	D3DXFillTexture(rayMap, makeRayMap, 0);

	D3DXCreateTextureFromFile(
		GetGraphicDevice()->GetDevice(), 
		"media/noise.dds", 
		&rayMap );

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
//	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/Head_Big_Ears.x")));
//	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/Head_Sad.x")));
	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/LandShark.x")));
//	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/wall_with_pillars.x")));
//	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/Cube2.x")));
//	m_kMeshObjectList.push_back( m_rpResouceManager->LoadBackGround<MeshObject>(_H("media/sp.x")));
	
	m_iFlag = 0;
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

	if(NULL != httpObject_ && httpObject_->Complete())
	{
		std::string result = httpObject_->GetResult();
	}

	{
		static int cnt=0;
		cnt++;
		std::string message;
		for(int i=0; i<10;i++)
		{
			message+=cformat("%d(%d)",cnt,i);
		}

		msgpack::sbuffer buf;
		buf.write(message.c_str(),message.length());

		Network::Get()->SendData(NULL,"message",buf);
	}

	// 通信更新
	Network::Get()->Update();


	if(true == GamePad(0).IsPressed(InputDevice::GamePad::A))
	{
		m_rpActionController->ChangeAction(_H("Punch"));
	}
	if(true == GamePad(0).IsPressed(InputDevice::GamePad::B))
	{
		m_rpActionController->ChangeAction(_H("Kick"));
	}

	if(true == GamePad(0).IsPressed(InputDevice::GamePad::X))
	{
		m_iFlag++;
		m_iFlag%=2;
	}

	if(false == GamePad(0).IsOn(InputDevice::GamePad::Y))
	{
		f+=static_cast<float>(Time::GetFrameDeltaTimeSecond())*0.1f;
	//	f=3.14f*0.5f;
		l=sin(f)*2.f+300.f;
	}


	m_rpActionController->Update();

	GetInputDevice()->DebugPrintGamePad(0,5);

	// エフェクト内のワールドビュー射影変換行列を設定
	D3DXMATRIX WVP;
	D3DXMATRIX mat;
	D3DXMatrixLookAtLH( &View, &D3DXVECTOR3(l*sin(f),-l*cos(f*2.f),-l*cos(f)), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
	D3DXMatrixIdentity( &WVP );
	D3DXMatrixIdentity( &mat );
	WVP = mat * View * Proj;

	pEffect->SetMatrix( "m_WVP", &WVP );
	pEffect->SetMatrix( "m_View", &View );
	pEffect->SetMatrix( "m_Proj", &Proj );
	pEffect->SetVector( "m_LightDir", &D3DXVECTOR4(1,1,1,0) );
	pEffect->SetVector( "m_Ambient" , &D3DXVECTOR4(0.8f,0.8f,0.8f,1.0));
	pEffect->SetInt( "m_iFlag" , m_iFlag);

	LPDIRECT3DDEVICE9 d3ddevice = Framework::Get().GetGraphicDevice()->GetDevice();
	HRESULT hr;
	hr = d3ddevice->SetRenderTarget(0, surColor);
	hr = d3ddevice->SetRenderTarget(1, surNormalDepth);
	hr = d3ddevice->SetRenderTarget(2, surPosition);

    d3ddevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	// 描画開始
	pEffect->SetTechnique( "RenderScene0" );
	UINT numPass;
	pEffect->Begin( &numPass, 0 );

	pEffect->BeginPass(0);
	for(MeshObjectList::iterator ite = m_kMeshObjectList.begin(); m_kMeshObjectList.end()!=ite; ite++)
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
	d3ddevice->SetRenderTarget(1, NULL);
	d3ddevice->SetRenderTarget(2, NULL);
    d3ddevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    d3ddevice->SetTexture(0, rayMap );
    d3ddevice->SetTexture(1, texNormalDepth );
    d3ddevice->SetTexture(2, texColor );
    d3ddevice->SetTexture(3, texPosition );

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