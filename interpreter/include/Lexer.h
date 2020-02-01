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
	char currentChar;
	int pos;
	std::map<std::string, Token> reservedKeywords;
	bool currentlyInComment;
	int line;

	void Advance();
	char Peek();

	Token HandleReserved(int line);
public:
	Lexer(const std::string& input);
	~Lexer() = default;

	Token NextToken();
};

#endif