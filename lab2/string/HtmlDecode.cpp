#include "HtmlDecode.hpp"
#include <unordered_map>

// " (двойная кавычка) заменяется на &quot;
// ' (апостроф) заменяется на &apos;
// < (знак меньше) заменяется на &lt;
// > (знак больше) заменяется на &gt;
// & (амперсанд) заменяется на &amp;

namespace DecodeConfig
{
const int maxEntityLength = 6;
const std::unordered_map<std::string, char> entityMap = {
	{ "&quot", '"' },
	{ "&apos", '\'' },
	{ "&lt", '<' },
	{ "&gt", '>' },
	{ "&amp", '&' }
};
}; // namespace DecodeConfig

std::string HtmlDecode(std::string const& html)
{
	std::string result;
	std::string currentEntity;
	for (char ch : html)
	{
		if (ch == '&')
		{
			if (!currentEntity.empty()) result += currentEntity;
			currentEntity = "&";
		}
		else if (ch == ';' && !currentEntity.empty())
		{
			auto it = DecodeConfig::entityMap.find(currentEntity);
			if (it != DecodeConfig::entityMap.end())
			{
				result += it->second;
			}
			else
			{
				result += currentEntity + ch;
			}
			currentEntity.clear();
		}
		else if (currentEntity.length() > DecodeConfig::maxEntityLength)
		{
			result += currentEntity + ch;
			currentEntity.clear();
		}
		else if (!currentEntity.empty())
		{
			currentEntity += ch;
		}
		else
		{
			result += ch;
		}
	}
    result += currentEntity; 
	return result;
}