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
#include "Ast/VariableArrayDeclarationAssignNode.h"
#include "Ast/VariableArrayUsageNode.h"
#include "Ast/ForEachNode.h"
#include "Ast/ForINode.h"
#include "Ast/BooleanNode.h"
#include "Ast/VariableArrayAssignNode.h"
#include "Ast/IfNode.h"
#include "Ast/WhileNode.h"
#include <Ast/ReturnNode.h>
#include <Ast/VariableIncrementDecrementNode.h>
#include <Ast/VariableCompoundAssignNode.h>
#include <Ast/DoWhileNode.h>

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
		Exit(this->currentToken, "Unexpected token type. Expected %s but got %s",
			Helper::ToString(tokenType).c_str(), Helper::ToString(this->currentToken.GetTokenType()).c_str());
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
		Exit(this->currentToken, "Expected token 'Function' but got '%s'",
			Helper::ToString(this->currentToken.GetTokenType()).c_str());
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
		Exit(this->currentToken.GetFileName(), -1, "No 'Main' method found. Cannot execute program");
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
		Ref<Node> expression = Expression();

		return std::make_shared<VariableDeclarationAssignNode>(this->lexer.GetFileName(), varLine, variableName, expression);
	}

	auto arrayValues = std::vector<Ref<Node>>();
	TokenType arrayType;

	Advance(SquareLeft);

	if (this->currentToken.GetTokenType() == TokenType::SquareRight)
	{
		Exit(this->currentToken, "Array '%s' cannot be empty", variableName.c_str());
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
			Exit(this->currentToken, "Unsupported array type '%s'",
				Helper::ToString(this->currentToken.GetTokenType()).c_str());
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

	std::string functionName = currentToken.GetValue();
	// "Call", because it is used when the next char is "(", which is when calling a function (possible parameters etc.)
	Advance(Call);

	Advance(ParanLeft);
	std::vector<std::string> parameters;
	while (this->currentToken.GetTokenType() != TokenType::ParanRight)
	{
		std::string paramName = this->currentToken.GetValue();
		Advance(TokenType::Name);

		parameters.push_back(paramName);

		if (this->currentToken.GetTokenType() == TokenType::Comma)
		{
			Advance(TokenType::Comma);
		}
	}
	Advance(TokenType::ParanRight);

	Ref<Node> block = ParseBlock();

	return std::make_shared<FunctionNode>(functionName, block, parameters);
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

	return std::make_shared<FunctionCallNode>(this->lexer.GetFileName(), line, functionName, parameters);
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
	std::vector<Ref<Node>> statements = ParseStatements();
	Advance(CurlyRight);

	return std::make_shared<BlockNode>(statements);
}

std::vector<Ref<Node>> Parser::ParseStatements()
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

	// for i in arrayVar
	// for i in Range(4)
	if (this->currentToken.GetTokenType() != TokenType::Int)
	{
		Ref<Node> expression = Expression();
		Ref<Node> block = ParseBlock();

		return std::make_shared<ForEachNode>(this->lexer.GetFileName(), line, variableName, expression, block);
	}
	// for i in 0..5
	else 
	{
		int from = std::stoi(this->currentToken.GetValue());
		Advance(TokenType::Int);

		Advance(TokenType::Point);
		Advance(TokenType::Point);

		int to = std::stoi(this->currentToken.GetValue());
		Advance(TokenType::Int);

		int step = 1;

		if (this->currentToken.GetTokenType() == TokenType::Step)
		{
			Advance(TokenType::Step);

			step = std::stoi(this->currentToken.GetValue());
			Advance(TokenType::Int);
		}

		Ref<Node> block = ParseBlock();

		return std::make_shared<ForINode>(this->lexer.GetFileName(), line, variableName, from, to, step, block);
	}
}

Ref<Node> Parser::ParseWhile()
{
	auto line = this->currentToken.GetLine();
	Advance(TokenType::While);

	Ref<Node> expression = Expression();

	Ref<Node> block = ParseBlock();

	return std::make_shared<WhileNode>(this->lexer.GetFileName(), line, expression, block);
}

Ref<Node> Parser::ParseDoWhile()
{
	auto line = this->currentToken.GetLine();
	Advance(TokenType::Do);

	Ref<Node> block = ParseBlock();

	Advance(TokenType::While);

	Ref<Node> expression = Expression();

	return std::make_shared<DoWhileNode>(this->lexer.GetFileName(), line, expression, block);
}

Ref<Node> Parser::ParseIf()
{
	auto line = this->currentToken.GetLine();
	Advance(TokenType::If);

	Ref<Node> expression = Expression();

	Ref<Node> trueBlock = ParseBlock();
	std::map<Ref<Node>, Ref<Node>> elseIfBlocks;
	Ref<Node> elseBlock = nullptr;

	if (this->currentToken.GetTokenType() == TokenType::Else)
	{
		Advance(TokenType::Else);

		if (this->currentToken.GetTokenType() != TokenType::If)
		{
			elseBlock = ParseBlock();
		}
		else
		{
			Advance(TokenType::If);

			auto elseIfExpression = Expression();
			auto elseIfBlock = ParseBlock();
			elseIfBlocks.insert(std::pair<Ref<Node>, Ref<Node>>(elseIfExpression, elseIfBlock));

			while (this->currentToken.GetTokenType() == TokenType::Else)
			{
				Advance(TokenType::Else);

				if (this->currentToken.GetTokenType() == TokenType::If)
				{
					Advance(TokenType::If);

					auto innerElseIfExpression = Expression();
					auto innerElseIfBlock = ParseBlock();

					elseIfBlocks.insert(std::pair<Ref<Node>, Ref<Node>>(innerElseIfExpression, innerElseIfBlock));
				}
				else
				{
					elseBlock = ParseBlock();

					break;
				}
			}
		}
	}

	return std::make_shared<IfNode>(this->lexer.GetFileName(), line, expression, trueBlock, elseIfBlocks, elseBlock);
}

Ref<Node> Parser::ParseWhen()
{
	auto line = this->currentToken.GetLine();
	Advance(TokenType::When);
	Ref<Node> whenFactor = Factor();
	Advance(TokenType::CurlyLeft);

	std::map<Ref<Node>, Ref<Node>> cases;
	Ref<Node> elseBlock = nullptr;
	while (this->currentToken.GetTokenType() != TokenType::CurlyRight)
	{
		// Handle default (else) case, if only the arrow symbol is used with a block
		if (this->currentToken.GetTokenType() == TokenType::Arrow)
		{
			Advance(TokenType::Arrow);
			elseBlock = ParseBlock();
			break;
		}

		Ref<Node> value = Factor();
		Advance(TokenType::Arrow);
		Ref<Node> block = ParseBlock();

		Ref<Node> expression = std::make_shared<BooleanNode>(whenFactor, TokenType::Equals, value);
		cases.insert(std::pair<Ref<Node>, Ref<Node>>(expression, block));
	}

	Advance(TokenType::CurlyRight);

	if (cases.size() == 0)
	{
		Exit(this->currentToken, "'when' statement needs to have at least one case expression and block");
	}

	Ref<Node> firstExpression = cases.begin()->first;
	Ref<Node> firstBlock = cases.begin()->second;

	cases.erase(cases.begin());

	return std::make_shared<IfNode>(this->lexer.GetFileName(), line, firstExpression, firstBlock, cases, elseBlock);
}

Ref<Node> Parser::ParseReturn()
{
	Advance(TokenType::Return);

	Ref<Node> expression = Expression();

	return std::make_shared<ReturnNode>(expression);
}

Ref<Node> Parser::Factor()
{
	Token tmp = this->currentToken;
	if (tmp.GetTokenType() == Name)
	{
		std::string varName = this->currentToken.GetValue();
		Advance(Name);

		auto line = this->currentToken.GetLine();

		if (this->currentToken.GetTokenType() == PlusPlus)
		{
			Advance(PlusPlus);

			return std::make_shared<VariableIncrementDecrementNode>(this->lexer.GetFileName(), line, varName, 1);
		}
		else if (this->currentToken.GetTokenType() == MinusMinus)
		{
			Advance(MinusMinus);

			return std::make_shared<VariableIncrementDecrementNode>(this->lexer.GetFileName(), line, varName, -1);
		}
		else if (this->currentToken.GetTokenType() != SquareLeft)
		{
			return std::make_shared<VariableUsageNode>(this->lexer.GetFileName(), line, varName);
		}
		else
		{
			Advance(SquareLeft);

			if (this->currentToken.GetTokenType() != Int)
			{
				Exit(this->currentToken, "Array access index for '%s' can only be an int",
					varName.c_str());
			}

			int arrayIndex = std::stoi(this->currentToken.GetValue());
			if (arrayIndex < 0)
			{
				Exit(this->currentToken, "Array index for '%s' cannot be negative", varName.c_str());
			}
			Advance(Int);

			Advance(SquareRight);

			return std::make_shared<VariableArrayUsageNode>(this->lexer.GetFileName(), line, varName, arrayIndex);
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

		std::vector<Ref<Node>> expressions;

		size_t indexOfCurlyOpen = value.find('{');
		while (indexOfCurlyOpen != -1)
		{
			size_t indexOfCurlyClose = value.find('}', indexOfCurlyOpen);

			std::string varName = value.substr(indexOfCurlyOpen + 1, indexOfCurlyClose - indexOfCurlyOpen - 1);

			Lexer lexer(varName, this->lexer.GetFileName());
			Parser parser(lexer);

			try
			{
				expressions.push_back(parser.Expression());
			}
			catch (const std::exception& e)
			{
				std::string what(e.what());
				
				Exit(this->currentToken.GetFileName(), this->currentToken.GetLine(), what);
			}

			value = value.replace(indexOfCurlyOpen, (indexOfCurlyClose + 1) - indexOfCurlyOpen, std::string("$R$"));
			indexOfCurlyOpen = value.find('{');
		}

		return std::make_shared<StringNode>(this->lexer.GetFileName(), this->currentToken.GetLine(), value, expressions);
	}
	else if (tmp.GetTokenType() == Bool)
	{
		std::string value = this->currentToken.GetValue();
		Advance(Bool);

		return std::make_shared<BoolNode>(value);
	}

	Exit(this->currentToken, "Unexpected token '%s' ('%s')",
		Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetValue().c_str());

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

			Ref<Node> right = Expression();
			//Ref<Node> right = Term();
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

		int line = this->currentToken.GetLine();
		printf("Statement: %i\n", line);

		if (this->currentToken.GetTokenType() == Assign)
		{
			Advance(Assign);

			return std::make_shared<VariableAssignNode>(this->lexer.GetFileName(), line, varName, Expression());
		}
		else if (this->currentToken.GetTokenType() == PlusPlus)
		{
			Advance(PlusPlus);

			return std::make_shared<VariableIncrementDecrementNode>(this->lexer.GetFileName(), line, varName, 1);
		}
		else if (this->currentToken.GetTokenType() == Plus)
		{
			Advance(Plus);
			Advance(Assign);

			return std::make_shared<VariableCompoundAssignNode>(this->lexer.GetFileName(), line, varName, Expression(), TokenType::Plus);
		}
		else if (this->currentToken.GetTokenType() == MinusMinus)
		{
			Advance(MinusMinus);

			return std::make_shared<VariableIncrementDecrementNode>(this->lexer.GetFileName(), line, varName, -1);
		}
		else if (this->currentToken.GetTokenType() == Minus)
		{
			auto line = this->currentToken.GetLine();

			Advance(Minus);
			Advance(Assign);

			return std::make_shared<VariableCompoundAssignNode>(this->lexer.GetFileName(), line, varName, Expression(), TokenType::Minus);
		}
		else if (this->currentToken.GetTokenType() == Multiply)
		{
			auto line = this->currentToken.GetLine();

			Advance(Multiply);
			Advance(Assign);

			return std::make_shared<VariableCompoundAssignNode>(this->lexer.GetFileName(), line, varName, Expression(), TokenType::Multiply);
		}
		else if (this->currentToken.GetTokenType() == Divide)
		{
			auto line = this->currentToken.GetLine();

			Advance(Divide);
			Advance(Assign);

			return std::make_shared<VariableCompoundAssignNode>(this->lexer.GetFileName(), line, varName, Expression(), TokenType::Divide);
		}
		else if (this->currentToken.GetTokenType() == SquareLeft)
		{
			Advance(TokenType::SquareLeft);

			if (this->currentToken.GetTokenType() != TokenType::Int)
			{
				Exit(this->currentToken, "Array access index for '%s' can only be an int", varName.c_str());
			}

			int arrayIndex = std::stoi(this->currentToken.GetValue());
			if (arrayIndex < 0)
			{
				Exit(this->currentToken, "Array index for '%s' cannot be negative", varName.c_str());
			}
			Advance(TokenType::Int);

			Advance(TokenType::SquareRight);

			Advance(TokenType::Assign);

			return std::make_unique<VariableArrayAssignNode>(this->lexer.GetFileName(), line, varName, arrayIndex, Expression());
		}
		else
		{
			return std::make_unique<VariableUsageNode>(this->lexer.GetFileName(), line, varName);
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
	else if (this->currentToken.GetTokenType() == Do)
	{
		return ParseDoWhile();
	}
	else if (this->currentToken.GetTokenType() == Var)
	{
		return ParseGlobalVariables();
	}
	else if (this->currentToken.GetTokenType() == If)
	{
		return ParseIf();
	}
	else if (this->currentToken.GetTokenType() == When)
	{
		return ParseWhen();
	}
	else if (this->currentToken.GetTokenType() == Return)
	{
		return ParseReturn();
	}

	Exit(this->currentToken, "Unexpected token '%s' ('%s')",
		Helper::ToString(this->currentToken.GetTokenType()).c_str(), this->currentToken.GetValue().c_str());

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