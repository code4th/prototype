#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "kickflipGraphicShaderObject.h"
#include "kickflipHashString.h"
#include <map>
#include <string>
/*
408 名前：デフォルトの名無しさん ：2005/12/12(月) 08:28:25
template< char c0 > 
struct static_hash1 { static const unsigned int value = (0*137+c0)%1987; }; 
template< char c0, char c1 > 
struct static_hash2 { static const unsigned int value = ((0*137+c0)*137+c1)%1987; }; 
template< char c0, char c1, char c2 > 
struct static_hash3 { static const unsigned int value = (((0*134+c0)*137+c1)*137+c2)%1987; }; 
template< char c0, char c1 = 0, char c2 = 0 > 
struct static_hash : static_hash3<c0,c1,c2> { }; 
template< char c0 > 
struct static_hash<c0> : static_hash1<c0> { }; 
template< char c0, char c1 > 
struct static_hash<c0,c1> : static_hash2<c0,c1> { }; 
int main() { 
char const *s = "ef"; 
unsigned int v = dynamic_hash(s); 
switch (v) { 
case static_hash<'a','b','c'>::value: 
std::cout << "abc" << std::endl; 
break; 
case static_hash<'e','f'>::value: 
std::cout << "ef" << std::endl; 
break; 
default: 
std::cout << "default" << std::endl; 
} 
} 
めんどいと思ったらBOOST MPL辺りを検索 
*/
/*
シェーダ管理
*/

namespace kickflip
{

	class GraphicShader
	{
	public:
		GraphicShader(void)
		{
		}
		virtual ~GraphicShader(void)
		{
		}
	private:
		SmartPtr(ShaderFile);
		class ShaderFile : public ReferenceObject
		{
		public:
			ShaderFile(void){}
			virtual ~ShaderFile(void){}
			typedef std::map<const unsigned int, GraphicShaderObjectRPtr>	ShaderObjectMap;	// シェーダーオブジェクトマップ
		private:
			ShaderObjectMap m_kMapShaderObject;
		public:
			const GraphicShaderObjectRPtr Load(const unsigned int iFlags)
			{
				ShaderObjectMap::iterator ite = m_kMapShaderObject.find(iFlags);
				if (m_kMapShaderObject.end() != ite) 
				{
					// 発見
					return ite->second;
				}
				// 作成
				GraphicShaderObjectRPtr rpShaderObject = new GraphicShaderObject();
				m_kMapShaderObject[iFlags] = rpShaderObject;

				return rpShaderObject;
			}
		};
		typedef std::map<const hash32, ShaderFileRPtr>	ShaderFileMap;		// シェーダーファイルマップ
		ShaderFileMap m_kMapShaderFile;

	public:
		const GraphicShaderObjectRPtr Load( const hashString fileName, const unsigned long iFlags )
		{
			ShaderFileMap::iterator ite = m_kMapShaderFile.find(fileName.hash);

			if (m_kMapShaderFile.end() != ite) 
			{
					// 発見
				return ite->second->Load(iFlags);
			}

			// 作成
			ShaderFileRPtr rpShaderFile = new ShaderFile();
			m_kMapShaderFile[fileName.hash] = rpShaderFile;

			return rpShaderFile->Load(iFlags);

		}
/*
		const GraphicShaderObjectRPtr Load( const char* fileName, const unsigned long iFlags )
		{
			return Load(hashStringDynamic(fileName), iFlags);
		}
*/
	};
}

