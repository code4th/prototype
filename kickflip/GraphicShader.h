#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "GraphicShaderObject.h"
#include "HashString.h"
#include <map>
#include <string>
/*
シェーダ管理
*/

namespace kickflip
{
	SmartPtr(GraphicShader);
	class GraphicShader :public ReferenceObject
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

