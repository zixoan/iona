/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Parser.h"
#include "Ast/MainNode.h"
#include "Ast/VariableDeclarationAssignNode.h"
#include "Ast/FunctionNode.h"
#include "Ast/BlockNode.h"
#include "Ast/FunctionCallNode.h"
#include <Ast/Literal/StringNode.h>
#include <Ast/Literal/IntNode.h>
#include <Ast/VariableUsageNode.h>
#include <Ast/VariableAssignNode.h>
#include <Ast/Literal/FloatNode.h>
#include <Ast/Literal/BoolNode.h>
#include <Ast/BinaryNode.h>
#include <Ast/VariableArrayDeclarationAssignNode.h>
#include <Ast/VariableArrayUsageNode.h>
#include <Ast/ForEachNode.h>
#include <Ast/BooleanNode.h>

Parser::Parser(Lexer& lexer) 
	: lexer(lexer), currentToken(lexer.NextToken())
{

}

void Parser::Advance(TokenType tokenType)
{
	if (this->currentToken.GetTokenType() == tokenType)
	{
		this->currentToken = this->lexer.NextToken();
	}
	else
	{
		Exit("Unexpected token type. Expected %s but got %s on line %i", 
			Helper::ToString(tokenType).c_str(), Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetLine());
	}
}

Ref<Node> Parser::Parse()
{
	return ParseMainFile();
}

Ref<Node> Parser::ParseMainFile()
{
	std::vector<Ref<Node>> globalVariables;
	std::vector<Ref<Node>> globalFunctions;

	while (this->currentToken.GetTokenType() == TokenType::Var)
	{
		globalVariables.push_back(ParseGlobalVariables());
	}

	if (this->currentToken.GetTokenType() != TokenType::Function)
	{
		Exit("Expected token 'Function' but got '%s' at line %i", 
			Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetLine());
	}

	while (this->currentToken.GetTokenType() == TokenType::Function)
	{
		globalFunctions.push_back(ParseFunction());
	}

	// Make sure a Main function exists and it's the first one
	size_t mainFunctionIndex = -1;
	for (size_t i = 0; i < globalFunctions.size(); i++)
	{
		Ref<FunctionNode> fun = std::dynamic_pointer_cast<FunctionNode>(globalFunctions[i]);
		if (fun->GetName() == MainFunctionName)
		{
			mainFunctionIndex = i;
		}
	}

	if (mainFunctionIndex == -1)
	{
		Exit("No 'Main' method found. Cannot execute program");
	}

	std::swap(globalFunctions[0], globalFunctions[mainFunctionIndex]);

	return std::make_shared<MainNode>(globalVariables, globalFunctions);
}

Ref<Node> Parser::ParseGlobalVariables()
{
	int varLine = this->currentToken.GetLine();
	Advance(TokenType::Var);

	std::string variableName = this->currentToken.GetValue();
	Advance(TokenType::Name);

	Advance(TokenType::Assign);
	
	// First check for non array declaration
	if (this->currentToken.GetTokenType() != SquareLeft)
	{
		Token variableValue = this->currentToken;
		if (!IsVariableType(variableValue.GetTokenType()))
		{
			Exit("Type '%s' of variable '%s' is not a valid variable type (line %i)",
				Helper::ToString(variableValue.GetTokenType()).c_str(), variableName.c_str(), variableValue.GetLine());
		}

		Advance(variableValue.GetTokenType());
		return std::make_shared<VariableDeclarationAssignNode>(variableName, variableValue);
	}

	auto arrayValues = std::vector<Ref<Node>>();
	TokenType arrayType;

	Advance(SquareLeft);

	switch (this->currentToken.GetTokenType())
	{
		case Int:
			arrayType = IntArray;
			break;
		case String:
			arrayType = StringArray;
			break;
		case Bool:
			arrayType = BoolArray;
			break;
		case Float:
			arrayType = FloatArray;
			break;
		default:
			Exit("Unsupported array type '%s'", Helper::ToString(this->currentToken.GetTokenType()).c_str());
	}

	arrayValues.push_back(Factor());

	while (this->currentToken.GetTokenType() != SquareRight)
	{
		if (this->currentToken.GetTokenType() == Comma)
		{
			Advance(Comma);
		}

		arrayValues.push_back(Factor());
	}

	Advance(SquareRight);

	if (arrayValues.empty())
	{
		Exit("Arrays must have at least one value");
	}

	return std::make_shared<VariableArrayDeclarationAssignNode>(variableName, arrayType, arrayValues);
}

Ref<Node> Parser::ParseFunction()
{
	Advance(TokenType::Function);

	// TODO: returnToken is ignored for now
	//Token returnToken = this->currentToken;
	//Advance(returnToken.GetTokenType());

	std::string functionName = currentToken.GetValue();
	// "Call", because it is used when the next char is "(", which is when calling a function (possible parameters etc.)
	Advance(Call);

	Advance(ParanLeft);
	std::vector<Ref<Node>> parameters;
	while (this->currentToken.GetTokenType() != TokenType::ParanRight)
	{
		// TODO: Simplify function parameters and remove the need from eg. var 
		parameters.push_back(ParseGlobalVariables());

		if (this->currentToken.GetTokenType() == TokenType::Comma)
		{
			Advance(TokenType::Comma);
		}
	}
	Advance(ParanRight);

	Ref<Node> block = ParseBlock();

	return std::make_shared<FunctionNode>(functionName, Token(TokenType::None, ""), block, parameters);
}

Ref<Node> Parser::ParseFunctionCall()
{
	std::string functionName = this->currentToken.GetValue();
	Advance(Call);

	Advance(ParanLeft);
	std::vector<Ref<Node>> parameters;
	while (this->currentToken.GetTokenType() != TokenType::ParanRight)
	{
		parameters.push_back(Factor());

		// TODO: Handle cases where there is a colon after a variable
		// TODO: eg. Test(a, a:)
		if (this->currentToken.GetTokenType() == TokenType::Comma)
		{
			Advance(TokenType::Comma);
		}
	}
	Advance(ParanRight);

	return std::make_shared<FunctionCallNode>(functionName, parameters);
}

Ref<Node> Parser::ParseBlock()
{
	Advance(CurlyLeft);
	std::vector<Ref<Node>> statements = ParseFunctionStatements();
	Advance(CurlyRight);

	return std::make_shared<BlockNode>(statements);
}

std::vector<Ref<Node>> Parser::ParseFunctionStatements()
{
	std::vector<Ref<Node>> statements;

	while (this->currentToken.GetTokenType() != TokenType::CurlyRight)
	{
		statements.push_back(Statement());
	}

	return statements;
}

Ref<Node> Parser::ParseFor()
{
	Advance(TokenType::For);

	std::string variableName = this->currentToken.GetValue();
	Advance(TokenType::Name);

	Advance(TokenType::In);

	std::string arrayName = this->currentToken.GetValue();
	Advance(TokenType::Name);

	Ref<Node> block = ParseBlock();

	return std::make_shared<ForEachNode>(variableName, arrayName, block);
}

Ref<Node> Parser::Factor()
{
	Token tmp = this->currentToken;
	if (tmp.GetTokenType() == Name)
	{
		std::string varName = this->currentToken.GetValue();
		Advance(Name);

		if (this->currentToken.GetTokenType() != SquareLeft)
		{
			return std::make_shared<VariableUsageNode>(varName);
		}
		else
		{
			Advance(SquareLeft);

			if (this->currentToken.GetTokenType() != Int)
			{
				Exit("Array access index can only be an int (line %i)", this->currentToken.GetLine());
			}

			unsigned int arrayIndex = std::stoul(this->currentToken.GetValue());
			Advance(Int);

			Advance(SquareRight);

			return std::make_shared<VariableArrayUsageNode>(varName, arrayIndex);
		}
	}
	else if (tmp.GetTokenType() == Call)
	{
		return ParseFunctionCall();
	}
	else if (tmp.GetTokenType() == Int)
	{
		std::string value = this->currentToken.GetValue();
		Advance(Int);

		return std::make_shared<IntNode>(value);
	}
	else if (tmp.GetTokenType() == Float)
	{
		std::string value = this->currentToken.GetValue();
		Advance(Float);

		return std::make_shared<FloatNode>(value);
	}
	else if (tmp.GetTokenType() == String)
	{
		std::string value = this->currentToken.GetValue();
		Advance(String);

		return std::make_shared<StringNode>(value);
	}
	else if (tmp.GetTokenType() == Bool)
	{
		std::string value = this->currentToken.GetValue();
		Advance(Bool);

		return std::make_shared<BoolNode>(value);
	}

	Exit("Unexpected token '%s' ('%s') in line %i",
		Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetValue().c_str(), this->currentToken.GetLine());

	return nullptr;
}

Ref<Node> Parser::Term()
{
	Ref<Node> result = Factor();

	while (currentToken.GetTokenType() == Multiply ||
		currentToken.GetTokenType() == Divide ||
		currentToken.GetTokenType() == Equals)
	{
		Token tmp = currentToken;
		if (tmp.GetTokenType() == Multiply || tmp.GetTokenType() == Divide)
		{
			Advance(tmp.GetTokenType());

			Ref<Node> right = Factor();
			result = std::make_shared<BinaryNode>(result, tmp.GetTokenType(), right);
		}
		else if (tmp.GetTokenType() == Equals)
		{
			Advance(tmp.GetTokenType());

			Ref<Node> right = Factor();
			result = std::make_shared<BooleanNode>(result, tmp.GetTokenType(), right);
		}
	}

	return result;
}

Ref<Node> Parser::Statement()
{
	if (this->currentToken.GetTokenType() == Call)
	{
		return ParseFunctionCall();
	}
	else if (this->currentToken.GetTokenType() == Name)
	{
		std::string varName = this->currentToken.GetValue();
		Advance(Name);

		if (this->currentToken.GetTokenType() == Assign)
		{
			Advance(Assign);

			return std::make_shared<VariableAssignNode>(varName, Expression());
		}
		else
		{
			return std::make_unique<VariableUsageNode>(varName);
		}
	}
	else if (this->currentToken.GetTokenType() == For)
	{
		return ParseFor();
	}
	else if (this->currentToken.GetTokenType() == Var)
	{
		return ParseGlobalVariables();
	}

	Exit("Unexpected token '%s' ('%s') in line %i", 
		Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetValue().c_str(), this->currentToken.GetLine());

	return nullptr;
}

Ref<Node> Parser::Expression()
{
	Ref<Node> result = Term();

	while (currentToken.GetTokenType() == Plus || currentToken.GetTokenType() == Minus)
	{
		Token tmp = currentToken;
		if (tmp.GetTokenType() == Plus)
		{
			Advance(Plus);
		}
		else if (tmp.GetTokenType() == Minus)
		{
			Advance(Minus);
		}
		else if (tmp.GetTokenType() == Multiply)
		{
			Advance(Multiply);
		}
		else if (tmp.GetTokenType() == Divide)
		{
			Advance(Divide);
		}

		Ref<Node> right = Term();
		result = std::make_shared<BinaryNode>(result, tmp.GetTokenType(), right);
	}

	return result;
}