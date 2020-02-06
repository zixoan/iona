/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Lexer.h"

#include <utility>
#include <iostream>

Lexer::Lexer(const std::string& input, const std::string& fileName)
	: input(input), fileName(fileName), pos(0), currentlyInComment(false), line(1)
{
	this->currentChar = this->input.at(this->pos);
	this->reservedKeywords.insert(std::pair<std::string, Token>("func", Token(Function, "func")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("call", Token(Call, "call")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("return", Token(Return, "return")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("int", Token(Int, "int")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("var", Token(Var, "var")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("string", Token(String, "string")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("true", Token(Bool, "true")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("false", Token(Bool, "false")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("for", Token(For, "for")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("in", Token(In, "in")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("while", Token(While, "while")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("if", Token(If, "if")));
	this->reservedKeywords.insert(std::pair<std::string, Token>("else", Token(Else, "else")));
}

void Lexer::Advance()
{
	this->pos++;
	if (this->pos > this->input.length() - 1)
	{
		this->currentChar = EOF;
	}
	else
	{
		this->currentChar = this->input.at(this->pos);
	}
}

char Lexer::Peek()
{
	int peekPos = pos + 1;
	if (peekPos > this->input.length() - 1)
	{
		return EOF;
	}
	else
	{
		return this->input.at(peekPos);
	}
}

Token Lexer::HandleReserved(int line)
{
	std::stringstream result;

	while (this->currentChar != EOF && std::isalpha(this->currentChar))
	{
		result << this->currentChar;
		Advance();
	}

	std::string possibleKeywordString = result.str();

	auto possibleKeyword = this->reservedKeywords.find(possibleKeywordString);
	if (possibleKeyword == this->reservedKeywords.end())
	{
		// Check if we got a function call (also used for function declaration)
		if (this->currentChar == '(')
		{
			return Token(Call, possibleKeywordString, GetFormattedFileLine());
		}
		else
		{
			return Token(Name, possibleKeywordString, GetFormattedFileLine());
		}
	}

	possibleKeyword->second.SetLine(GetFormattedFileLine());

	return possibleKeyword->second;
}

Token Lexer::NextToken()
{
	while (this->currentChar != EOF)
	{
		// Source files should end with \n (line feed) instead of \r (carriage return)
		// TODO: Verify this as one of the first steps
		if (this->currentChar == '\n')
		{
			line++;
		}
		
		if (this->currentChar == '/' && Peek() == '/')
		{
			Advance();
			Advance();

			while (this->currentChar != EOF && this->currentChar != '\n')
			{
				Advance();
			}
		}

		if (this->currentChar == '/' && Peek() == '*')
		{
			Advance();
			Advance();

			while (this->currentChar != EOF && (this->currentChar != '*' && Peek() != '/'))
			{
				Advance();
			}

			Advance();
			Advance();
		}

		// Whitespace
		if (std::isspace(this->currentChar))
		{
			while (this->currentChar != EOF && std::isspace(this->currentChar))
			{
				Advance();
			}

			// Go to top checks so comments etc. are detected after spaces/new lines
			continue;
		}

		// Ignore accidentally placed semicolons
		if (this->currentChar == ';')
		{
			Advance();
		}

		if (std::isalpha(this->currentChar))
		{
			return HandleReserved(line);
		}

		if (std::isdigit(this->currentChar) || (this->currentChar == '-' && std::isdigit(this->Peek())))
		{
			std::stringstream number;
			bool hasFloatPoint = false;

			if (this->currentChar == '-')
			{
				number << this->currentChar;

				Advance();
			}

			while (this->currentChar != EOF && (std::isdigit(this->currentChar) || this->currentChar == '.'))
			{
				number << this->currentChar;
				Advance();

				if (this->currentChar == '.' && !hasFloatPoint)
				{
					hasFloatPoint = true;
				}
				else if (this->currentChar == '.' && hasFloatPoint)
				{
					// TODO: Show an error message or just remove it silently?
					Advance();
					break;
				}
			}

			TokenType intOrFloat = !hasFloatPoint ? TokenType::Int : TokenType::Float;

			return Token(intOrFloat, number.str(), GetFormattedFileLine());
		}

		switch (this->currentChar)
		{
			case '.':
				Advance();
				return Token(Point, ".", GetFormattedFileLine());
			case ':':
				Advance();
				return Token(Colon, ":", GetFormattedFileLine());
			case '(':
				Advance();
				return Token(ParanLeft, "(", GetFormattedFileLine());
			case ')':
				Advance();
				return Token(ParanRight, ")", GetFormattedFileLine());
			case '{':
				Advance();
				return Token(CurlyLeft, "{", GetFormattedFileLine());
			case '}':
				Advance();
				return Token(CurlyRight, "}", GetFormattedFileLine());
			case '[':
				Advance();
				return Token(SquareLeft, "[", GetFormattedFileLine());
			case ']':
				Advance();
				return Token(SquareRight, "]", GetFormattedFileLine());
			case '>':
				if (Peek() != '=')
				{
					Advance();
					return Token(GreaterThan, ">", GetFormattedFileLine());
				}

				Advance();
				Advance();

				return Token(GreaterEqualThan, ">=", GetFormattedFileLine());
			case '<':
				if (Peek() != '=')
				{
					Advance();
					return Token(LessThan, "<", GetFormattedFileLine());
				}

				Advance();
				Advance();

				return Token(LessEqualThan, "<=", GetFormattedFileLine());
			case '=':
				if (Peek() != '=')
				{
					Advance();
					return Token(Assign, "=", GetFormattedFileLine());
				}

				Advance();
				Advance();

				return Token(Equals, "==", GetFormattedFileLine());
			case '!':
				if (Peek() == '=')
				{
					Advance();
					Advance();
					return Token(NotEquals, "!=", GetFormattedFileLine());
				}

				Advance();

				return Token(ExclamationMark, "!", GetFormattedFileLine());
			case '+':
				Advance();
				return Token(Plus, "+", GetFormattedFileLine());
			case '-':
				Advance();
				return Token(Minus, "-", GetFormattedFileLine());
			case '*':
				Advance();
				return Token(Multiply, "*", GetFormattedFileLine());
			case '/':
				Advance();
				return Token(Divide, "/", GetFormattedFileLine());
			case ',':
				Advance();
				return Token(Comma, ",", GetFormattedFileLine());
			case '"':
			{
				Advance();

				std::stringstream string;
				while (this->currentChar != EOF && this->currentChar != '"')
				{
					string << this->currentChar;
					Advance();
				}

				Advance();

				return Token(String, string.str(), GetFormattedFileLine());
			}
			default:
				std::cerr << GetFormattedFileLine() << " Invalid character '" << this->currentChar << "'" << std::endl;
				Advance();
				break;
		}
	}

	return Token(None, "NONE", GetFormattedFileLine());
}