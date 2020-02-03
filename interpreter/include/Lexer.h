/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <sstream>
#include <map>
#include "Token.h"

class Lexer
{
private:
	std::string input;
	std::string fileName;
	char currentChar;
	int pos;
	std::map<std::string, Token> reservedKeywords;
	bool currentlyInComment;
	int line;

	void Advance();
	char Peek();

	Token HandleReserved(int line);
	
	std::string GetFormattedFileLine()
	{
		std::stringstream s;
		s << fileName << "(line " << line << "):";
		return s.str();
	}
public:
	Lexer(const std::string& input, const std::string& fileName);
	~Lexer() = default;

	Token NextToken();
};

#endif