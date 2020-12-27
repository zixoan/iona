/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Interpreter.h"
#include "Standard.h"
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

Interpreter::Interpreter(const std::vector<std::string>& args, const Parser& parser)
	: Interpreter(args, parser, std::make_shared<InterpreterScope>())
{
}

Interpreter::Interpreter(const std::vector<std::string>& args, const Parser& parser, const Ref<InterpreterScope>& scope)
	: parser(parser), root(nullptr)
{
	this->scopes.push_back(scope);

	RegisterInternalFunctions();
	RegisterInternalVariables();

	std::vector<VariableType> argsValues;
	argsValues.reserve(args.size());
	for (auto& arg : args)
	{
		argsValues.push_back({ TokenType::String, arg });
	}
	this->scopes.back()->DeclareVariable("ARGS", TokenType::StringArray, argsValues);
}

void Interpreter::RegisterInternalFunctions()
{
	this->internalFunctions.Register("WriteLine", Iona::Console::WriteLine, 1, { { 0, { TokenType::Int, TokenType::Float, TokenType::String, TokenType::Bool, TokenType::Array } } });
	this->internalFunctions.Register("ReadLine", Iona::Console::ReadLine, 0);
	this->internalFunctions.Register("ReadInt", Iona::Console::ReadInt, 0);
	this->internalFunctions.Register("ReadFloat", Iona::Console::ReadFloat, 0);

	this->internalFunctions.Register("ToUpperCase", Iona::String::ToUpperCase, 1, { { 0, { TokenType::String } } });
	this->internalFunctions.Register("ToLowerCase", Iona::String::ToLowerCase, 1, { { 0, { TokenType::String } } });
	this->internalFunctions.Register("StartsWith", Iona::String::StartsWith, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
	this->internalFunctions.Register("EndsWith", Iona::String::EndsWith, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
	this->internalFunctions.Register("Contains", Iona::String::Contains, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
	this->internalFunctions.Register("Split", Iona::String::Split, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
	this->internalFunctions.Register("Trim", Iona::String::Trim, 1, { { 0, { TokenType::String } } });

	this->internalFunctions.Register("Size", Iona::Core::Size, 1, { { 0, { TokenType::String, TokenType::Array } } });
	this->internalFunctions.Register("Empty", Iona::Core::Empty, 1, { { 0, { TokenType::String, TokenType::Array } } });
	this->internalFunctions.Register("Random", Iona::Core::Random, 2, { { 0, { TokenType::Int } }, { 1, { TokenType::Int } } });
	this->internalFunctions.Register("Range", Iona::Core::Range, 1, { { 0, { TokenType::Int } } });
	this->internalFunctions.Register("Reverse", Iona::Core::Reverse, 1, { { 0, { TokenType::IntArray, TokenType::StringArray, TokenType::BoolArray, TokenType::FloatArray } } });
	this->internalFunctions.Register("ToString", Iona::Core::ToString, 1, { { 0, { TokenType::String, TokenType::Int, TokenType::Float, TokenType::Bool, TokenType::Array } } });

	this->internalFunctions.Register("Min", Iona::Math::Min, 2, { { 0, { TokenType::Int, TokenType::Float } }, { 1, { TokenType::Int, TokenType::Float } } });
	this->internalFunctions.Register("Max", Iona::Math::Max, 2, { { 0, { TokenType::Int, TokenType::Float } }, { 1, { TokenType::Int, TokenType::Float } } });

	this->internalFunctions.Register("FileExists", Iona::File::FileExists, 1, { { 0, { TokenType::String } } });
	this->internalFunctions.Register("FileRead", Iona::File::FileRead, 1, { { 0, { TokenType::String } } });
	this->internalFunctions.Register("FileWrite", Iona::File::FileWrite, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
	this->internalFunctions.Register("FileCopy", Iona::File::FileCopy, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
	this->internalFunctions.Register("FileReadLines", Iona::File::FileReadLines, 1, { { 0, { TokenType::String } } });
	this->internalFunctions.Register("FileWriteLines", Iona::File::FileWriteLines, 3, { { 0, { TokenType::String } }, { 1, { TokenType::StringArray } }, { 2, { TokenType::Bool } } });
	this->internalFunctions.Register("FileList", Iona::File::FileList, 2, { { 0, { TokenType::String } }, { 1, { TokenType::String } } });
}

void Interpreter::RegisterInternalVariables()
{
	this->scopes.back()->DeclareVariable("PI", { TokenType::Float, (float) M_PI });

	this->scopes.back()->DeclareVariable("INT_MIN", { TokenType::Int, std::numeric_limits<int>::min() });
	this->scopes.back()->DeclareVariable("INT_MAX", { TokenType::Int, std::numeric_limits<int>::max() });

	this->scopes.back()->DeclareVariable("FLOAT_MIN", { TokenType::Float, std::numeric_limits<float>::min() });
	this->scopes.back()->DeclareVariable("FLOAT_MAX", { TokenType::Float, std::numeric_limits<float>::max() });
}

Ref<InterpreterScope> Interpreter::FindScopeOfVariable(const std::string& variableName)
{
	Ref<InterpreterScope> scope = nullptr;
	for (size_t i = this->scopes.size(); i-- > 0; )
	{
		if (this->scopes[i]->IsDeclared(variableName))
		{
			scope = this->scopes[i];
			break;
		}
	}

	return scope;
}

void Interpreter::Interpret()
{
	auto start = std::chrono::high_resolution_clock::now();

	this->root = this->parser.Parse();
	
	auto end = std::chrono::high_resolution_clock::now();

	IONA_LOG("\nParsing took %llims (%llins)\n\n",
		std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

	start = std::chrono::high_resolution_clock::now();

	this->root->Accept(shared_from_this());

	end = std::chrono::high_resolution_clock::now();

	IONA_LOG("\n\nInterpreting took %llims (%llins)\n",
		std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

void Interpreter::Visit(const Ref<MainNode>& n)
{
	for (auto globalVariable : n->GetGlobalVariables())
	{
		globalVariable->Accept(shared_from_this());
	}

	for (auto& globalFunction : n->GetGlobalFunctions())
	{
		Ref<FunctionNode> fun = std::dynamic_pointer_cast<FunctionNode>(globalFunction);

		this->globalFunctions.insert(std::pair<std::string, Ref<FunctionNode>>(
			fun->GetName(),
			fun)
		);
	}

	// Parser has ensured that there is a main function at index zero
	n->GetGlobalFunctions().at(0)->Accept(shared_from_this());
}

void Interpreter::Visit(const Ref<VariableDeclarationAssignNode>& n)
{
	n->GetExpression()->Accept(shared_from_this());

	if (!IsVariableType(this->currentVariable.type))
	{
		Exit(n->GetFileName(), n->GetLine(), "Type '%s' of variable '%s' is not a valid variable type",
			Helper::ToString(this->currentVariable.type).c_str(), n->GetName().c_str());
	}

	this->scopes.back()->DeclareVariable(n->GetName(), this->currentVariable);
}

void Interpreter::Visit(const Ref<FunctionNode>& n)
{
	this->scopes.push_back(std::make_shared<InterpreterScope>());

	// We need to register possible function parameters after the possible function scope was pushed
	for (size_t i = 0; i < this->currentFunctionCallParams.size(); ++i)
	{
		// Accept the parameter types (eg. StringNode)
		this->currentFunctionCallParams[i]->Accept(shared_from_this());

		this->scopes.back()->DeclareVariable(this->currentFunctionCallFunctionParams[i], this->currentVariable);
	}

	n->GetBlock()->Accept(shared_from_this());

	this->scopes.pop_back();
}

void Interpreter::Visit(const Ref<BlockNode>& n)
{
	for (auto& statement : n->GetStatements())
	{
		statement->Accept(shared_from_this());
	}
}

void Interpreter::Visit(const Ref<FunctionCallNode>& n)
{
	auto result = this->globalFunctions.find(n->GetName());

	if (result != this->globalFunctions.end())
	{
		Ref<FunctionNode> function = result->second;

		if (n->GetParameters().size() != function->GetParameters().size())
		{
			Exit(n->GetFileName(), n->GetLine(), "Function call parameter count (%i) is not matching expected parameter count (%i) of function '%s'",
				n->GetParameters().size(), function->GetParameters().size(), function->GetName().c_str());
		}

		this->currentFunctionCallParams = n->GetParameters();
		this->currentFunctionCallFunctionParams = function->GetParameters();

		function->Accept(shared_from_this());
	}
	// Internal function handling
	else
	{
		if (this->internalFunctions.Exists(n->GetName()))
		{
			std::vector<VariableType> in;
			in.reserve(n->GetParameters().size());
			VariableType out;

			for (auto& parameter : n->GetParameters())
			{
				parameter->Accept(shared_from_this());

				in.push_back(this->currentVariable);
			}

			this->internalFunctions.Call(n->GetFileName(), n->GetLine(), n->GetName(), in, out);

			// We need to update the current variable with the returned one
			this->currentVariable = std::move(out);
		}
		else
		{
			Exit(n->GetFileName(), n->GetLine(), "Function '%s' not found",
				n->GetName().c_str());
		}
	}
}

void Interpreter::Visit(const Ref<ForEachNode>& n)
{
	n->GetExpression()->Accept(shared_from_this());

	if (!IsVariableArrayType(this->currentVariable.type))
	{
		Exit(n->GetFileName(), n->GetLine(), "For loop can only loop over arrays, but in type is '%s'", Helper::ToString(this->currentVariable.type).c_str());
	}

	this->scopes.push_back(std::make_shared<InterpreterScope>());

	this->scopes.back()->DeclareVariable(n->GetVariableName(), this->currentVariable.type);

	std::vector<VariableType> values = std::any_cast<std::vector<VariableType>>(this->currentVariable.value);
		
	for (auto& value : values)
	{
		// TODO: Check variable type??
		this->scopes.back()->UpdateVariable(n->GetVariableName(), value);
			
		n->GetBlock()->Accept(shared_from_this());
	}

	this->scopes.pop_back();
}

void Interpreter::Visit(const Ref<ForINode>& n)
{
	this->scopes.push_back(std::make_shared<InterpreterScope>());

	this->scopes.back()->DeclareVariable(n->GetVariableName(), TokenType::Int);

	for (int i = n->GetFrom(); i < n->GetTo(); i += n->GetStep())
	{
		this->scopes.back()->UpdateVariable(n->GetVariableName(), { TokenType::Int, i });

		n->GetBlock()->Accept(shared_from_this());
	}

	this->scopes.pop_back();
}

void Interpreter::Visit(const Ref<StringNode>& n)
{
	std::string value = n->GetValue();

	for (auto& expression : n->GetExpressions())
	{
		expression->Accept(shared_from_this());

		int index = value.find("$R$");
		value = value.replace(index, 3, Iona::ToStringInternal(this->currentVariable));
	}

	VariableType v;
	v.type = TokenType::String;
	v.value = value;

	this->currentVariable = std::move(v);
}

void Interpreter::Visit(const Ref<IntNode>& n)
{
	VariableType v;
	v.type = TokenType::Int;
	v.value = n->GetValue();

	this->currentVariable = std::move(v);
}

void Interpreter::Visit(const Ref<FloatNode>& n)
{
	VariableType v;
	v.type = TokenType::Float;
	v.value = n->GetValue();

	this->currentVariable = std::move(v);
}

void Interpreter::Visit(const Ref<BoolNode>& n)
{
	VariableType v;
	v.type = TokenType::Bool;
	v.value = n->GetValue();

	this->currentVariable = std::move(v);
}

void Interpreter::Visit(const Ref<VariableUsageNode>& n)
{
	auto scope = FindScopeOfVariable(n->GetName());

	if (scope != nullptr)
	{
		this->currentVariable = *scope->GetVariable(n->GetName());
	}
	else
	{
		Exit(n->GetFileName(), n->GetLine(), "Variable '%s' not declared in this scope",
			n->GetName().c_str());
	}
}

void Interpreter::Visit(const Ref<VariableIncrementDecrementNode>& n)
{
	auto scope = FindScopeOfVariable(n->GetName());

	if (scope != nullptr)
	{
		auto variable = scope->GetVariable(n->GetName());
		
		if (variable->type == TokenType::Int)
		{
			variable->value = std::any_cast<int>(variable->value) + n->GetValue();
		}
		else if (variable->type == TokenType::Float)
		{
			variable->value = std::any_cast<float>(variable->value) + n->GetValue();
		}
		else
		{
			Exit(n->GetFileName(), n->GetLine(), "Variable increments are only supported with int and float, but got %s",
				Helper::ToString(variable->type).c_str());
		}

		this->currentVariable = *variable;
	}
	else
	{
		Exit(n->GetFileName(), n->GetLine(), "Variable '%s' not declared in this scope",
			n->GetName().c_str());
	}
}

void Interpreter::Visit(const Ref<VariableAssignNode>& n)
{
	auto innerScope = FindScopeOfVariable(n->GetName());
	if (innerScope == nullptr)
	{
		Exit(n->GetFileName(), n->GetLine(), "Variable '%s' not declared in this scope", n->GetName().c_str());
	}

	n->GetExpression()->Accept(shared_from_this());

	auto variable = innerScope->GetVariable(n->GetName());

	// We only need to check for same type, because the variable needs to be declared
	// so it was already checked that it's a valid variable type
	if (this->currentVariable.type != variable->type)
	{
		Exit(n->GetFileName(), n->GetLine(), "New value of variable '%s' needs to be of type '%s', but is '%s'",
			n->GetName().c_str(), Helper::ToString(variable->type).c_str(), Helper::ToString(this->currentVariable.type).c_str());
	}

	innerScope->UpdateVariable(n->GetName(), this->currentVariable);
}

void Interpreter::Visit(const Ref<VariableCompoundAssignNode>& n)
{
	auto innerScope = FindScopeOfVariable(n->GetName());
	if (innerScope == nullptr)
	{
		Exit(n->GetFileName(), n->GetLine(), "Variable '%s' not declared in this scope", n->GetName().c_str());
	}

	n->GetExpression()->Accept(shared_from_this());

	auto variable = innerScope->GetVariable(n->GetName());

	// We only need to check for same type, because the variable needs to be declared
	// so it was already checked that it's a valid variable type
	if (this->currentVariable.type != variable->type)
	{
		Exit(n->GetFileName(), n->GetLine(), "New value of variable '%s' needs to be of type '%s', but is '%s'",
			n->GetName().c_str(), Helper::ToString(variable->type).c_str(), Helper::ToString(this->currentVariable.type).c_str());
	}

	VariableType vt;

	if (variable->type == TokenType::Int)
	{
		vt.type = TokenType::Int;

		switch (n->GetOperation())
		{
			case TokenType::Plus:
				vt.value = std::any_cast<int>(variable->value) + std::any_cast<int>(this->currentVariable.value);
				break;
			case TokenType::Minus:
				vt.value = std::any_cast<int>(variable->value) - std::any_cast<int>(this->currentVariable.value);
				break;
			case TokenType::Multiply:
				vt.value = std::any_cast<int>(variable->value) * std::any_cast<int>(this->currentVariable.value);
				break;
			case TokenType::Divide:
				vt.value = std::any_cast<int>(variable->value) / std::any_cast<int>(this->currentVariable.value);
				break;
		}
	}
	else if (variable->type == TokenType::Float)
	{
		vt.type = TokenType::Float;

		switch (n->GetOperation())
		{
			case TokenType::Plus:
				vt.value = std::any_cast<float>(variable->value) + std::any_cast<float>(this->currentVariable.value);
				break;
			case TokenType::Minus:
				vt.value = std::any_cast<float>(variable->value) - std::any_cast<float>(this->currentVariable.value);
				break;
			case TokenType::Multiply:
				vt.value = std::any_cast<float>(variable->value) * std::any_cast<float>(this->currentVariable.value);
				break;
			case TokenType::Divide:
				vt.value = std::any_cast<float>(variable->value) / std::any_cast<float>(this->currentVariable.value);
				break;
		}
	}
	else
	{
		Exit(n->GetFileName(), n->GetLine(), "Variable increment assignments are only supported with int and float, but got %s",
			Helper::ToString(variable->type).c_str());
	}

	this->currentVariable = vt;

	innerScope->UpdateVariable(n->GetName(), vt);
}

void Interpreter::Visit(const Ref<BinaryNode>& n)
{
	n->GetLeft()->Accept(shared_from_this());

	VariableType leftVariable = this->currentVariable;

	n->GetRight()->Accept(shared_from_this());

	VariableType rightVariable = this->currentVariable;

	VariableType resultVariable;

	if (leftVariable.type == TokenType::Int && rightVariable.type == TokenType::Int)
	{
		resultVariable.type = TokenType::Int;
			
		if (n->GetOperant() == TokenType::Plus)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) + std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::Minus)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) - std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::Multiply)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) * std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::Divide)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) / std::any_cast<int>(rightVariable.value);
		}
	}
	else if (leftVariable.type == TokenType::Float && rightVariable.type == TokenType::Float)
	{
		resultVariable.type = TokenType::Float;

		if (n->GetOperant() == TokenType::Plus)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) + std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::Minus)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) - std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::Multiply)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) * std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::Divide)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) / std::any_cast<float>(rightVariable.value);
		}
	}
	else if (leftVariable.type == TokenType::String && rightVariable.type == TokenType::String)
	{
		resultVariable.type = TokenType::String;
			
		if (n->GetOperant() == TokenType::Plus)
		{
			resultVariable.value = std::any_cast<std::string>(leftVariable.value) + std::any_cast<std::string>(rightVariable.value);
		}
		else
		{
			Exit(n->GetFileName(), n->GetLine(), "Invalid arithmetic string operator '%s'", Helper::ToString(n->GetOperant()).c_str());
		}
	}

	this->currentVariable = std::move(resultVariable);
}

void Interpreter::Visit(const Ref<BooleanNode>& n)
{
	n->GetLeft()->Accept(shared_from_this());

	VariableType leftVariable = this->currentVariable;

	n->GetRight()->Accept(shared_from_this());

	VariableType rightVariable = this->currentVariable;

	VariableType resultVariable;
	resultVariable.type = TokenType::Bool;

	if (leftVariable.type == TokenType::Int && rightVariable.type == TokenType::Int)
	{
		if (n->GetOperant() == TokenType::Equals)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) == std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::NotEquals)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) != std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) < std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) > std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessEqualThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) <= std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterEqualThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) >= std::any_cast<int>(rightVariable.value);
		}
	}
	else if (leftVariable.type == TokenType::Float && rightVariable.type == TokenType::Float)
	{
		if (n->GetOperant() == TokenType::Equals)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) == std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::NotEquals)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) != std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) < std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) > std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessEqualThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) <= std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterEqualThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) >= std::any_cast<float>(rightVariable.value);
		}
	}
	else if (leftVariable.type == TokenType::Int && rightVariable.type == TokenType::Float)
	{
		if (n->GetOperant() == TokenType::Equals)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) == std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::NotEquals)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) != std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) < std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) > std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessEqualThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) <= std::any_cast<float>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterEqualThan)
		{
			resultVariable.value = std::any_cast<int>(leftVariable.value) >= std::any_cast<float>(rightVariable.value);
		}
	}
	else if (leftVariable.type == TokenType::Float && rightVariable.type == TokenType::Int)
	{
		if (n->GetOperant() == TokenType::Equals)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) == std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::NotEquals)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) != std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) < std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) > std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::LessEqualThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) <= std::any_cast<int>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::GreaterEqualThan)
		{
			resultVariable.value = std::any_cast<float>(leftVariable.value) >= std::any_cast<int>(rightVariable.value);
		}
	}
	else if (leftVariable.type == TokenType::String && rightVariable.type == TokenType::String)
	{
		if (n->GetOperant() == TokenType::Equals)
		{
			resultVariable.value = std::any_cast<std::string>(leftVariable.value) == std::any_cast<std::string>(rightVariable.value);
		}
		else if (n->GetOperant() == TokenType::NotEquals)
		{
			resultVariable.value = std::any_cast<std::string>(leftVariable.value) != std::any_cast<std::string>(rightVariable.value);
		}
	}
	else
	{
		resultVariable.value = false;
	}

	this->currentVariable = std::move(resultVariable);
}

void Interpreter::Visit(const Ref<VariableArrayDeclarationAssignNode>& n)
{
	std::vector<VariableType> values;

	for (auto& value : n->GetValues())
	{
		value->Accept(shared_from_this());

		VariableType variable;

		if (n->GetArrayType() == TokenType::IntArray)
		{
			variable.type = TokenType::Int;
			variable.value = this->currentVariable.value;
		}
		else if (n->GetArrayType() == TokenType::FloatArray)
		{
			variable.type = TokenType::Float;
			variable.value = this->currentVariable.value;
		}
		else if (n->GetArrayType() == TokenType::StringArray)
		{
			variable.type = TokenType::String;
			variable.value = this->currentVariable.value;
		}
		else if (n->GetArrayType() == TokenType::BoolArray)
		{
			variable.type = TokenType::Bool;
			variable.value = this->currentVariable.value;
		}

		values.push_back(variable);
	}

	this->scopes.back()->DeclareVariable(n->GetName(), n->GetArrayType(), values);
}

void Interpreter::Visit(const Ref<VariableArrayUsageNode>& n)
{
	auto scope = FindScopeOfVariable(n->GetName());

	if (scope != nullptr)
	{
		auto a = scope->GetVariable(n->GetName());

		auto arrayVar = std::any_cast<std::vector<VariableType>>(scope->GetVariable(n->GetName())->value);
		if (n->GetIndex() > arrayVar.size() - 1)
		{
			Exit(n->GetFileName(), n->GetLine(), "Array index %i is higher than the max array index of %i", n->GetIndex(), arrayVar.size() - 1);
		}

		this->currentVariable = std::move(arrayVar.at(n->GetIndex()));
	}
	else
	{
		Exit(n->GetFileName(), n->GetLine(), "Variable '%s' not declared", n->GetName().c_str());
	}
}

void Interpreter::Visit(const Ref<VariableArrayAssignNode>& n)
{
	auto innerScope = FindScopeOfVariable(n->GetName());
	if (innerScope == nullptr)
	{
		Exit(n->GetFileName(), n->GetLine(), "Array variable '%s' not declared in this scope", n->GetName().c_str());
	}

	n->GetExpression()->Accept(shared_from_this());

	auto variable = innerScope->GetVariable(n->GetName());

	auto arrayValues = std::any_cast<std::vector<VariableType>>(variable->value);
	// TODO: Acces at zero without check is ok at the moment, because 
	// we cannot have empty arrays -> but this might change
	if (this->currentVariable.type != arrayValues.at(0).type)
	{
		Exit(n->GetFileName(), n->GetLine(), "New value of array variable '%s' needs to be of type '%s', but is '%s'",
			n->GetName().c_str(), Helper::ToString(variable->type).c_str(), Helper::ToString(this->currentVariable.type).c_str());
	}

	if (n->GetIndex() > arrayValues.size() - 1)
	{
		Exit(n->GetFileName(), n->GetLine(), "Array index %i is higher than the max array index of %i", n->GetIndex(), arrayValues.size() - 1);
	}

	innerScope->UpdateVariable(n->GetName(), n->GetIndex(), this->currentVariable);
}

void Interpreter::Visit(const Ref<IfNode>& n)
{
	n->GetExpression()->Accept(shared_from_this());

	if (this->currentVariable.type != TokenType::Bool)
	{
		Exit(n->GetFileName(), n->GetLine(), "Expression from an if needs to be a boolean result");
	}

	if (std::any_cast<bool>(this->currentVariable.value))
	{
		n->GetTrueBlock()->Accept(shared_from_this());
	}
	else
	{
		bool elseIfBlockExecuted = false;
		for (const auto& [expression, block] : n->GetElseIfBlocks())
		{
			expression->Accept(shared_from_this());

			if (std::any_cast<bool>(this->currentVariable.value))
			{
				block->Accept(shared_from_this());

				elseIfBlockExecuted = true;
				break;
			}
		}

		if (!elseIfBlockExecuted && n->GetElseBlock() != nullptr)
		{
			n->GetElseBlock()->Accept(shared_from_this());
		}
	}
}

void Interpreter::Visit(const Ref<WhileNode>& n)
{
	n->GetExpression()->Accept(shared_from_this());

	if (this->currentVariable.type != TokenType::Bool)
	{
		Exit(n->GetFileName(), n->GetLine(), "Expression from a while needs to be a boolean result");
	}

	this->scopes.push_back(std::make_shared<InterpreterScope>());

	while (std::any_cast<bool>(this->currentVariable.value))
	{
		n->GetBlock()->Accept(shared_from_this());

		n->GetExpression()->Accept(shared_from_this());
	}

	this->scopes.pop_back();
}

void Interpreter::Visit(const Ref<DoWhileNode>& n)
{
	do
	{
		n->GetBlock()->Accept(shared_from_this());

		n->GetExpression()->Accept(shared_from_this());

		if (this->currentVariable.type != TokenType::Bool)
		{
			Exit(n->GetFileName(), n->GetLine(), "Expression from a do while needs to be a boolean result");
		}
	} while (std::any_cast<bool>(this->currentVariable.value));
}

void Interpreter::Visit(const Ref<ReturnNode>& n)
{
	n->GetExpression()->Accept(shared_from_this());
}
