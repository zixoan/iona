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
#include "Ast/Literal/StringNode.h"
#include "Ast/Literal/IntNode.h"
#include "Ast/VariableUsageNode.h"
#include "Ast/VariableAssignNode.h"
#include "Ast/Literal/FloatNode.h"
#include "Ast/Literal/BoolNode.h"
#include "Ast/BinaryNode.h"
#include "Ast/VariableArrayDeclarationAssignNode.h"
#include "Ast/VariableArrayUsageNode.h"
#include "Ast/ForEachNode.h"
#include "Ast/BooleanNode.h"
#include "Ast/VariableArrayAssignNode.h"
#include "Ast/IfNode.h"
#include "Ast/WhileNode.h"

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
		Exit("%s Expected token 'Function' but got '%s'", 
			this->currentToken.GetLine(), Helper::ToString(this->currentToken.GetTokenType()).c_str());
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
	auto varLine = this->currentToken.GetLine();
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
			Exit("%s Type '%s' of variable '%s' is not a valid variable type",
				variableValue.GetLine(), Helper::ToString(variableValue.GetTokenType()).c_str(), variableName.c_str());
		}

		Advance(variableValue.GetTokenType());
		return std::make_shared<VariableDeclarationAssignNode>(variableName, variableValue);
	}

	auto arrayValues = std::vector<Ref<Node>>();
	TokenType arrayType;

	Advance(SquareLeft);

	if (this->currentToken.GetTokenType() == TokenType::SquareRight)
	{
		Exit("%s Array '%s' cannot be empty",
			this->currentToken.GetLine(), variableName.c_str());
	}

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
			Exit("%s Unsupported array type '%s'", 
				this->currentToken.GetLine(), Helper::ToString(this->currentToken.GetTokenType()).c_str());
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
		// TODO: Simplify function parameters and remove the need for eg. var 
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
	auto line = this->currentToken.GetLine();
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

	return std::make_shared<FunctionCallNode>(line, functionName, parameters);
}

Ref<Node> Parser::ParseBlock()
{
	// Support one line statement blocks without curly brackets
	if (this->currentToken.GetTokenType() != TokenType::CurlyLeft)
	{
		std::vector<Ref<Node>> statements;
		statements.push_back(Statement());
		return std::make_shared<BlockNode>(statements);
	}

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
	auto line = this->currentToken.GetLine();
	Advance(TokenType::For);

	std::string variableName = this->currentToken.GetValue();
	Advance(TokenType::Name);

	Advance(TokenType::In);

	std::string arrayName = this->currentToken.GetValue();
	Advance(TokenType::Name);

	Ref<Node> block = ParseBlock();

	return std::make_shared<ForEachNode>(line, variableName, arrayName, block);
}

Ref<Node> Parser::ParseWhile()
{
	Advance(TokenType::While);

	Ref<Node> expression = Expression();

	Ref<Node> block = ParseBlock();

	return std::make_shared<WhileNode>(expression, block);
}

Ref<Node> Parser::ParseIf()
{
	Advance(TokenType::If);

	Ref<Node> expression = Expression();

	Ref<Node> trueBlock = ParseBlock();
	Ref<Node> falseBlock = nullptr;

	if (this->currentToken.GetTokenType() == TokenType::Else)
	{
		Advance(TokenType::Else);

		falseBlock = ParseBlock();
	}

	return std::make_shared<IfNode>(expression, trueBlock, falseBlock);
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
				Exit("%s Array access index for '%s' can only be an int", 
					this->currentToken.GetLine(), varName.c_str());
			}

			int arrayIndex = std::stoi(this->currentToken.GetValue());
			if (arrayIndex < 0)
			{
				Exit("%s Array index for '%s' cannot be negative", this->currentToken.GetLine(), varName.c_str());
			}
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
		auto line = this->currentToken.GetLine();
		std::string value = this->currentToken.GetValue();
		Advance(String);

		return std::make_shared<StringNode>(line, value);
	}
	else if (tmp.GetTokenType() == Bool)
	{
		std::string value = this->currentToken.GetValue();
		Advance(Bool);

		return std::make_shared<BoolNode>(value);
	}

	Exit("%s Unexpected token '%s' ('%s')",
		this->currentToken.GetLine(), Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetValue().c_str());

	return nullptr;
}

Ref<Node> Parser::Term()
{
	Ref<Node> result = Factor();

	while (currentToken.GetTokenType() == Multiply || currentToken.GetTokenType() == Divide || 
		currentToken.GetTokenType() == Equals || currentToken.GetTokenType() == NotEquals || currentToken.GetTokenType() == LessThan || 
		currentToken.GetTokenType() == GreaterThan || currentToken.GetTokenType() == GreaterEqualThan || currentToken.GetTokenType() == LessEqualThan)
	{
		Token tmp = currentToken;
		if (tmp.GetTokenType() == Multiply || tmp.GetTokenType() == Divide)
		{
			Advance(tmp.GetTokenType());

			Ref<Node> right = Factor();
			result = std::make_shared<BinaryNode>(result, tmp.GetTokenType(), right);
		}
		else if (tmp.GetTokenType() == Equals || tmp.GetTokenType() == NotEquals || currentToken.GetTokenType() == LessThan || 
			currentToken.GetTokenType() == GreaterThan || currentToken.GetTokenType() == GreaterEqualThan || currentToken.GetTokenType() == LessEqualThan)
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
		else if (this->currentToken.GetTokenType() == SquareLeft)
		{
			Advance(TokenType::SquareLeft);

			if (this->currentToken.GetTokenType() != TokenType::Int)
			{
				Exit("%s Array access index for '%s' can only be an int", this->currentToken.GetLine(), varName.c_str());
			}

			int arrayIndex = std::stoi(this->currentToken.GetValue());
			if (arrayIndex < 0)
			{
				Exit("%s Array index for '%s' cannot be negative", this->currentToken.GetLine(), varName.c_str());
			}
			Advance(TokenType::Int);

			Advance(TokenType::SquareRight);

			Advance(TokenType::Assign);

			return std::make_unique<VariableArrayAssignNode>(varName, arrayIndex, Expression());
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
	else if (this->currentToken.GetTokenType() == While)
	{
		return ParseWhile();
	}
	else if (this->currentToken.GetTokenType() == Var)
	{
		return ParseGlobalVariables();
	}
	else if (this->currentToken.GetTokenType() == If)
	{
		return ParseIf();
	}

	Exit("%s Unexpected token '%s' ('%s')",
		this->currentToken.GetLine(), Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetValue().c_str());

	return nullptr;
}

Ref<Node> Parser::Expression()
{
	Ref<Node> result = Term();

	while (currentToken.GetTokenType() == Plus || currentToken.GetTokenType() == Minus || 
		currentToken.GetTokenType() == Multiply || currentToken.GetTokenType() == Divide)
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