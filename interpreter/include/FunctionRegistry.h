/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FUNCTION_REGISTRY_H
#define FUNCTION_REGISTRY_H

#include <functional>
#include <stack>
#include <map>
#include "Core.h"
#include "TokenType.h"

using InternalFunctionCallback = std::function<void(std::vector<VariableType>& in, VariableType& out)>;

struct FunctionEntry
{
	InternalFunctionCallback function;
	unsigned int parameterCount;
	std::map<int, std::vector<TokenType>> functionParameters;
};

class FunctionRegistry
{
private:
	std::map<std::string, FunctionEntry> functions;
public:
	FunctionRegistry() = default;
	~FunctionRegistry() = default;

	void Register(const std::string& name, InternalFunctionCallback function, unsigned int parameterCount, std::map<int, std::vector<TokenType>> functionParameters = {});

	void Call(const std::string& fileName, int line, const std::string& name, std::vector<VariableType>& in, VariableType& out);

	bool Exists(const std::string& name);
};

#endif