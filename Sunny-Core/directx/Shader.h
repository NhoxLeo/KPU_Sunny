#pragma once

#include "../sunny.h"
#include "../maths/maths.h"
#include "../system/FileSystem.h"
#include "../utils/String.h"
#include "Common.h"
#include "Context.h"
#include "ShaderUniform.h"
#include "ShaderResource.h"

namespace sunny
{
	namespace directx
	{
#define SHADER_VERTEX_INDEX   0
#define SHADER_COLOR_INDEX    1

#define SHADER_UNIFORM_PROJECTION_MATRIX_NAME	"sunny_ProjectionMatrix"              // ����
#define SHADER_UNIFORM_VIEW_MATRIX_NAME			"sunny_ViewMatrix"                    //   ��
#define SHADER_UNIFORM_MODEL_MATRIX_NAME		"sunny_ModelMatrix"                   // ��

		struct ShaderErrorInfo
		{
			std::string profile;
			std::string message;
		};


		class Shader
		{
		public:
			static const Shader* s_currentlyBound;
			
		private:
			struct Data
			{
				ID3D11VertexShader* vertexShader;
				ID3D11PixelShader*   pixelShader;
				ID3DBlob* vs;
				ID3DBlob* ps;
			};

			std::string m_name;
			std::string m_filePath;
			mutable Data m_data;

			ShaderUniformBufferList m_VSUniformBuffers;                // ���� ���̴� ��� ���ۿ� ���� ���� (sunny_)
			ShaderUniformBufferList m_PSUniformBuffers;                // �ȼ� ���̴� ��� ���ۿ� ���� ���� (sunny_)

			ShaderUniformBufferDeclaration* m_VSUserUniformBuffer;     // ���� ���̴� ���� ��� ���ۿ� ���� ����
			ShaderUniformBufferDeclaration* m_PSUserUniformBuffer;     // �ʼ� ���̴� ���� ��� ���ۿ� ���� ����

			ShaderResourceList m_resources;                            // �ؽ�ó���� �ڿ�

			ShaderStructList m_structs;                                // ����ü ����

			ID3D11Buffer** m_VSConstantBuffers;                        // ���� ���̴� ��� ���۵�
			unsigned int m_VSConstantBuffersCount;

			ID3D11Buffer** m_PSConstantBuffers;                        // �ȼ� ���̴� ��� ���۵�
			unsigned int m_PSConstantBuffersCount;

		public:
			Shader(const std::string& name, const std::string& source);
			~Shader();

			inline Data& GetData() const { return m_data; }

			void Bind() const;

			void SetVSSystemUniformBuffer(unsigned char* data, unsigned int size, unsigned int slot = 0);
			void SetPSSystemUniformBuffer(unsigned char* data, unsigned int size, unsigned int slot = 0);

			void SetVSUserUniformBuffer(unsigned char* data, unsigned int size);
			void SetPSUserUniformBuffer(unsigned char* data, unsigned int size);

			inline const std::string& GetName()     const { return m_name;     };
			inline const std::string& GetFilePath() const { return m_filePath; };

		private:
			static ID3DBlob* Compile(const std::string& source, const std::string& profile, const std::string& main, ShaderErrorInfo& info);

			void Load(const std::string& source);

			std::string RemoveComments(const std::string& source);

			void Parse(const std::string& source);
			void ParseStruct(const std::string& block);
			void ParseCBuffer(const std::string& block);
			void ParseTexture(const std::string& statement);
			void ParseSamplerState(const std::string& statement);

			void CreateBuffers();

			ShaderStruct* FindStruct(const std::string& name);

		public:
			friend static Shader* CreateFromFile  (const std::string& name, const std::string& filepath, void* address = nullptr);
			friend static Shader* CreateFromSource(const std::string& name, const std::string& source);

			static bool TryCompile        (const std::string& source  , std::string& error);
			static bool TryCompileFromFile(const std::string& filepath, std::string& error);

			static const Shader* CurrentlyBound() { return s_currentlyBound; }
		};
	}
}