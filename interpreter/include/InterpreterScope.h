/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include <map>
#include <string>
#include <memory>
#include "Core.h"
#include "Token.h"

class InterpreterScope
{
private:
	std::map<std::string, Ref<VariableType>> variables;
public:
	InterpreterScope() = default;
	~InterpreterScope() = default;

	void DeclareVariable(const std::string& variableName, const TokenType& variableType);
	void DeclareVariable(const std::string& variableName, const VariableType& variable);
	void DeclareVariable(const std::string& variableName, const TokenType& arrayType, const std::vector<VariableType>& token);

	bool IsDeclared(const std::string& name);

	void UpdateVariable(const std::string& variableName, const VariableType& variableType);
	void UpdateVariable(const std::string& variableName, unsigned int arrayIndex, const VariableType& variableType);

	Ref<VariableType> GetVariable(const std::string& name);
};

#endif