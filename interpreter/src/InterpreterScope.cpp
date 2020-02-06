/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "InterpreterScope.h"

void InterpreterScope::DeclareVariable(const std::string& variableName, const TokenType& type)
{
	VariableType t;

	// Declare a variable with a default variable for the given type
	switch (type)
	{
		case TokenType::IntArray:
		case TokenType::Int:
		{
			t.type = TokenType::Int;
			t.value = (int) 0;

			IONA_LOG("Declared int: %s = 0\n", variableName.c_str());
			break;
		}
		case TokenType::StringArray:
		case TokenType::String:
		{
			t.type = TokenType::String;
			t.value = "";

			IONA_LOG("Declared string: %s = \"\"\n", variableName.c_str());
			break;
		}
		case TokenType::BoolArray:
		case TokenType::Bool:
		{
			t.type = TokenType::Bool;
			t.value = false;

			IONA_LOG("Declared bool: %s = false\n", variableName.c_str());
			break;
		}
		case TokenType::FloatArray:
		case TokenType::Float:
		{
			t.type = TokenType::Float;
			t.value = 0.0f;

			IONA_LOG("Declared float: %s = 0.0\n", variableName.c_str());
			break;
		}
	default:
		break;
	}

	this->variables[variableName] = std::make_shared<VariableType>(t);
}

void InterpreterScope::DeclareVariable(const std::string& variableName, const VariableType& variable)
{
	this->variables[variableName] = std::make_shared<VariableType>(variable);
}

void InterpreterScope::DeclareVariable(const std::string& variableName, const TokenType& arrayType, const std::vector<VariableType>& array)
{
	VariableType t;
	t.type = arrayType;
	t.value = array;

	this->variables[variableName] = std::make_shared<VariableType>(t);

	IONA_LOG("Declared array: %s\n", variableName.c_str());
}

bool InterpreterScope::IsDeclared(const std::string& name)
{
	return this->variables.find(name) != this->variables.end();
}

void InterpreterScope::UpdateVariable(const std::string& variableName, const VariableType& type)
{
	this->variables[variableName]->value = type.value;
}

void InterpreterScope::UpdateVariable(const std::string& variableName, int arrayIndex, const VariableType& type)
{
	auto array = this->variables[variableName]->value;
	auto arrayValues = std::any_cast<std::vector<VariableType>>(array);
	
	arrayValues[arrayIndex].value = type.value;
	
	this->variables[variableName]->value = arrayValues;
}

void InterpreterScope::UpdateVariable(const std::string& variableName, const std::vector<VariableType>& array)
{
	this->variables[variableName]->value = array;
}

Ref<VariableType> InterpreterScope::GetVariable(const std::string& name)
{
	return this->variables.find(name)->second;
}