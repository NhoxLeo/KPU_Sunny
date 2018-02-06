#include "String.h"

namespace sunny
{
	namespace utils
	{
		/* FindPosition */
		int FindStringPosition(const std::string& string, const std::string search, unsigned int offset)
		{
			const char* str = string.c_str() + offset;
			const char* found = strstr(str, search.c_str()); // strstr(����ڿ�, �˻��ҹ��ڿ�);
			                                                 // ã���� �ϴ� ���ڿ��� �߰ߵ� ù��° ��ġ�� �����͸� ��ȯ / ã�� ���ϸ� NULL ��ȯ
			
			return found == nullptr ? -1 : ((int)(found - str) + offset);
		}


		/* Range */
		std::string StringRange(const std::string& string, unsigned int start, unsigned int length)
		{
			return string.substr(start, length);
		}

		std::string RemoveStringRange(const std::string& string, unsigned int start, unsigned int length)
		{
			std::string result = string;
			
			return result.erase(start, length);
		}


		/* FindToken */
		const char* FindToken(const char* str, const std::string& token)
		{
			const char* t = str;

			while (t = strstr(t, token.c_str()))
			{
				// isspace: ���ڰ� ���� ���������� �Ǻ�

				// ù(��)���� or ���� �յ�
				bool left  = str == t || isspace(t[-1]);
				bool right = !t[token.size()] || isspace(t[token.size()]);

				if (left && right)
					return t;

				t += token.size();
			}

			return nullptr;
		}
		
		const char* FindToken(const std::string& string, const std::string token)
		{
			return FindToken(string.c_str(), token);
		}



		/* GetBlock */
		std::string GetBlock(const char* str, const char** outPosition)
		{
			const char* end = strstr(str, "}");

			if (!end)
				return std::string(str);

			if (outPosition)
				*outPosition = end; // ���� ���ڿ� �̵� (���� �����Ͷ� ����)

			unsigned int length = end - str + 1;

			return std::string(str, length);
		}

		std::string GetBlock(const std::string& string, unsigned int offset)
		{
			const char* str = string.c_str() + offset;

			return GetBlock(str);
		}


		/* GetStatment */
		std::string GetStatement(const char* str, const char** outPosition)
		{
			const char* end = strstr(str, ";");

			if (!end)
				return std::string(str);

			if (outPosition)
				*outPosition = end; // ���� ���ڿ� �̵� (���� �����Ͷ� ����)

			unsigned int length = end - str + 1;

			return std::string(str, length);
		}


		/* SplitString */
		std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
		{
			size_t start = 0;
			size_t end = string.find_first_of(delimiters);
			// find()          �־��� �κй��ڿ��� ó������ ��Ÿ���� ���� ã�´�
			// find_first_of() find()�� ������ �־��� ���ڰ� ó������ ��Ÿ���� ��ġ�� ã�´�	

			std::vector<std::string> result;
			
			while (end <= std::string::npos)
			{
				std::string token = string.substr(start, end - start);

				if (!token.empty())
					result.push_back(token);

				if (end == std::string::npos)
					break;

				start = end + 1;
				end = string.find_first_of(delimiters, start);
			}

			return result;
		}

		std::vector<std::string> SplitString(const std::string& string, const char delimiter)
		{
			return SplitString(string, std::string(1, delimiter));
		}

		std::vector<std::string> Tokenize(const std::string& string)
		{
			return SplitString(string, " \t\n");
		}

		std::vector<std::string> GetLines(const std::string& string)
		{
			return SplitString(string, "\n");
		}



		// Guitar
		bool StringContains(const std::string& string, const std::string& chars)
		{
			return string.find(chars) != std::string::npos;
		}

		bool StartsWith(const std::string& string, const std::string& start)
		{
			return string.find(start) == 0;
		}

		int NextInt(const std::string& string)
		{
			const char* str = string.c_str();

			for (unsigned i = 0; i < string.size(); ++i)
			{
				if (isdigit(string[i]))
					return atoi((&string[i]));
			}

			return -1;
		}
	}
}