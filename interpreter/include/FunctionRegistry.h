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

struct FunctionEntry
{
	std::function<void(std::stack<VariableType>& in, VariableType& out)> function;
	unsigned int parameterCount;
};

class FunctionRegistry
{
private:
	std::map<std::string, FunctionEntry> functions;
public:
	FunctionRegistry() = default;
	~FunctionRegistry() = default;

	void Register(const std::string& name, std::function<void(std::stack<VariableType>& in, VariableType& out)> functions, unsigned int parameterCount);

	void Call(const char* line, const std::string& name, std::stack<VariableType>& in, VariableType& out);

	bool Exists(const std::string& name);
};

#endif