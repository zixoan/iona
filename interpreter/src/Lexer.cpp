/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Lexer.h"

#include <iostream>
#include <Core.h>

Lexer::Lexer(const std::string& input, const std::string& fileName)
	: input(input), fileName(fileName), pos(0), line(1)
{
	this->currentChar = this->input.at(this->pos);
	this->reservedKeywords.insert(std::pair<std::string, Token>("func", Token(Function, "func", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("return", Token(Return, "return", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("int", Token(Int, "int", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("var", Token(Var, "var", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("true", Token(Bool, "true", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("false", Token(Bool, "false", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("for", Token(For, "for", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("in", Token(In, "in", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("step", Token(Step, "step", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("while", Token(While, "while", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("do", Token(Do, "do", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("if", Token(If, "if", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("else", Token(Else, "else", this->fileName)));
	this->reservedKeywords.insert(std::pair<std::string, Token>("when", Token(When, "when", this->fileName)));
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

Token Lexer::HandleReserved()
{
	std::stringstream result;

	while (this->currentChar != EOF && (std::isalpha(this->currentChar) || this->currentChar == '_'))
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
			return Token(Call, possibleKeywordString, this->fileName, this->line);
		}
		else
		{
			return Token(Name, possibleKeywordString, this->fileName, this->line);
		}
	}

	possibleKeyword->second.SetLine(this->line);

	return possibleKeyword->second;
}

Token Lexer::NextToken()
{
	while (this->currentChar != EOF)
	{
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
                if (this->currentChar == '\n')
                {
                    line++;
                }

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
			return HandleReserved();
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
					if (this->Peek() == '.')
					{
						break;
					}

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

			return Token(intOrFloat, number.str(), this->fileName, this->line);
		}

		switch (this->currentChar)
		{
			case '.':
				Advance();
				return Token(Point, ".", this->fileName, this->line);
			case ':':
				Advance();
				return Token(Colon, ":", this->fileName, this->line);
			case '(':
				Advance();
				return Token(ParanLeft, "(", this->fileName, this->line);
			case ')':
				Advance();
				return Token(ParanRight, ")", this->fileName, this->line);
			case '{':
				Advance();
				return Token(CurlyLeft, "{", this->fileName, this->line);
			case '}':
				Advance();
				return Token(CurlyRight, "}", this->fileName, this->line);
			case '[':
				Advance();
				return Token(SquareLeft, "[", this->fileName, this->line);
			case ']':
				Advance();
				return Token(SquareRight, "]", this->fileName, this->line);
			case '>':
				if (Peek() != '=')
				{
					Advance();
					return Token(GreaterThan, ">", this->fileName, this->line);
				}

				Advance();
				Advance();

				return Token(GreaterEqualThan, ">=", this->fileName, this->line);
			case '<':
				if (Peek() != '=')
				{
					Advance();
					return Token(LessThan, "<", this->fileName, this->line);
				}

				Advance();
				Advance();

				return Token(LessEqualThan, "<=", this->fileName, this->line);
			case '=':
				if (Peek() == '>')
				{
					Advance();
					Advance();
					return Token(Arrow, "=>", this->fileName, this->line);
				}
				else if (Peek() != '=')
				{
					Advance();
					return Token(Assign, "=", this->fileName, this->line);
				}

				Advance();
				Advance();

				return Token(Equals, "==", this->fileName, this->line);
			case '!':
				if (Peek() == '=')
				{
					Advance();
					Advance();
					return Token(NotEquals, "!=", this->fileName, this->line);
				}

				Advance();

				return Token(ExclamationMark, "!", this->fileName, this->line);
			case '+':
				if (Peek() == '+')
				{
					Advance();
					Advance();
					return Token(PlusPlus, "++", this->fileName, this->line);
				}

				Advance();
				return Token(Plus, "+", this->fileName, this->line);
			case '-':
				if (Peek() == '-')
				{
					Advance();
					Advance();
					return Token(MinusMinus, "--", this->fileName, this->line);
				}

				Advance();
				return Token(Minus, "-", this->fileName, this->line);
			case '*':
				Advance();
				return Token(Multiply, "*", this->fileName, this->line);
			case '/':
				Advance();
				return Token(Divide, "/", this->fileName, this->line);
			case ',':
				Advance();
				return Token(Comma, ",", this->fileName, this->line);
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

				return Token(String, string.str(), this->fileName, this->line);
			}
			default:
				Exit(this->fileName, this->line, "Invalid character '%c'", this->currentChar);
				break;
		}
	}

	return Token(None, "NONE", this->fileName, this->line);
}