#include "FunctionRegistry.h"

void FunctionRegistry::Register(const std::string& name, 
								std::function<void(std::vector<VariableType>& in, VariableType& out)> function, 
								unsigned int parameterCount, 
								std::map<int, std::vector<TokenType>> functionParameters)
{
	FunctionEntry entry;
	entry.function = function;
	entry.parameterCount = parameterCount;
	entry.functionParameters = functionParameters;

	this->functions.insert(std::pair<std::string, FunctionEntry>(name, entry));
}

void FunctionRegistry::Call(const char* line, const std::string& name, std::vector<VariableType>& in, VariableType& out)
{
	auto result = this->functions.find(name);
	if (result != this->functions.end())
	{
		if (in.size() == result->second.parameterCount)
		{
			unsigned char parameterCheckPassedCount = 0;

			// Check if input parameter types match
			for (size_t i = 0; i < in.size(); i++)
			{
				auto allowedParamTypes = result->second.functionParameters.at(i);
				for (size_t j = 0; j < allowedParamTypes.size(); j++)
				{
					if (allowedParamTypes.at(j) == in.at(i).type 
						|| (allowedParamTypes.at(j) == TokenType::Array && IsVariableArrayType(in.at(i).type)))
					{
						parameterCheckPassedCount++;
						// Functions can have multiple allowed types per parameter, so if one of the allowed types matched, 
						// we can just break and check the next parameter
						break;
					}
				}
			}

			if (parameterCheckPassedCount == result->second.parameterCount)
			{
				result->second.function(in, out);
			}
			else
			{
				// TODO: More details like what parameter index, which type etc.
				Exit("%s Function call parameter types are not matching the allowed types from function '%s'",
					line, name.c_str());
			}
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
