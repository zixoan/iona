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

void InterpreterScope::DeclareVariable(const std::string& variableName, const Token& token)
{
	VariableType t;

	switch (token.GetTokenType())
	{
		case TokenType::Int:
		{
			t.type = TokenType::Int;
			t.value = std::stoi(token.GetValue());

			IONA_LOG("Declared int: %s = %s\n", variableName.c_str(), token.GetValue().c_str());
			break;
		}
		case TokenType::Float:
		{
			t.type = TokenType::Float;
			t.value = std::stof(token.GetValue());

			IONA_LOG("Declared float: %s = %s\n", variableName.c_str(), token.GetValue().c_str());
			break;
		}
		case TokenType::String:
		{
			t.type = TokenType::String;
			t.value = token.GetValue();

			IONA_LOG("Declared string: %s = \"%s\"\n", variableName.c_str(), token.GetValue().c_str());
			break;
		}
		case TokenType::Bool:
		{
			t.type = TokenType::Bool;
			t.value = token.GetValue() == "true";

			IONA_LOG("Declared bool: %s = %s\n", variableName.c_str(), token.GetValue().c_str());
			break;
		}
	}

	this->variables[variableName] = std::make_shared<VariableType>(t);
}

void InterpreterScope::DeclareVariable(const std::string& variableName, const TokenType& arrayType, const std::vector<VariableType>& array)
{
	VariableType t;
	t.type = arrayType;
	t.value = array;

	this->variables[variableName] = std::make_shared<VariableType>(t);
}

bool InterpreterScope::IsDeclared(const std::string& name)
{
	return this->variables.find(name) != this->variables.end();
}

void InterpreterScope::UpdateVariable(const std::string& variableName, const VariableType& type)
{
	this->variables[variableName]->value = type.value;
}

void InterpreterScope::UpdateVariable(const std::string& variableName, const std::vector<VariableType>& array)
{
	this->variables[variableName]->value = array;
}

Ref<VariableType> InterpreterScope::GetVariable(const std::string& name)
{
	return this->variables.find(name)->second;
}