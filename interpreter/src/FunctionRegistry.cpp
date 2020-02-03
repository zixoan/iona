#include "FunctionRegistry.h"

void FunctionRegistry::Register(const std::string& name, std::function<void(std::stack<VariableType>& in, VariableType& out)> function, unsigned int parameterCount)
{
	this->functions.insert(std::pair<std::string, FunctionEntry>(name, { function, parameterCount }));
}

void FunctionRegistry::Call(const char* line, const std::string& name, std::stack<VariableType>& in, VariableType& out)
{
	auto result = this->functions.find(name);
	if (result != this->functions.end())
	{
		if (in.size() == result->second.parameterCount)
		{
			result->second.function(in, out);
		}
		else
		{
			Exit("%s Function call parameter count (%i) is not matching expected paramter count (%i) of function '%s'", 
				line, in.size(), result->second.parameterCount, name.c_str());
		}
	}
}

bool FunctionRegistry::Exists(const std::string& name)
{
	return this->functions.find(name) != this->functions.end();
}
