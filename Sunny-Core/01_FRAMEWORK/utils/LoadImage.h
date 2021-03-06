#pragma once

#include "../include.h"

namespace sunny
{
	namespace utils
	{
		unsigned char* LoadSunnyImage(const char*  filename, unsigned int* width = nullptr, unsigned int* height = nullptr, unsigned int* bits = nullptr, bool flipY = false);
		unsigned char* LoadSunnyImage(const std::string& filename, unsigned int* width = nullptr, unsigned int* height = nullptr, unsigned int* bits = nullptr, bool flipY = false);
		
		unsigned char* LoadSunnyRaw(const char* filename, unsigned int* width = nullptr, unsigned int* height = nullptr, unsigned int* bits = nullptr);
		unsigned char* LoadSunnyRaw(const std::string& filename, unsigned int* width = nullptr, unsigned int* height = nullptr, unsigned int* bits = nullptr);
	}
}