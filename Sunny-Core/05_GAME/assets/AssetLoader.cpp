#include "AssetLoader.h"

namespace game
{
	void AssetLoader::LoadModel(const std::string& jsonPath)
	{
		std::string txt = sunny::system::FileSystem::ReadTextFile(jsonPath);

		Json::Value  root;
		Json::Reader reader;


		std::string name;
		int textureLen;

		if (reader.parse(txt.c_str(), root))
		{
			for (int i = 0; i < root.size(); ++i)
			{
				ModelData* data = new ModelData();

				name       = root[i]["name"].asString();
				std::string model_path = root[i]["path"].asString();
				
				textureLen = root[i]["textures"].size();

				for (int texture = 0; texture < textureLen; ++texture)
				{
					std::string texture_path = root[i]["textures"][texture].asString();
					
					data->textures.push_back(texture_path);

					if (!sunny::graphics::TextureManager::Get(texture_path))
						sunny::graphics::TextureManager::Add(new sunny::directx::Texture2D(texture_path));
					
				}

				if (!sunny::graphics::ModelManager::Get(name))
					sunny::graphics::ModelManager::Add(name, new sunny::graphics::Model(model_path));


				AssetData::AddModelData(name, data);

			}
		}
		else
		{
			// Debug System
			std::cout << "--------------------------------" << std::endl;
			std::cout << "LoadModel ���� : " << jsonPath << std::endl;
			std::cout << reader.getFormattedErrorMessages() << std::endl;
			std::cout << "--------------------------------" << std::endl;

			exit(1);
		}
	}
}