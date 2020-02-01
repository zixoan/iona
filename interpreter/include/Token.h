/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

class Token
{
private:
	TokenType tokenType;
	std::string value;

	int line;
public:
	Token();
	Token(const TokenType& tokenType, const std::string& value, int line = -1);
	~Token() = default;

	TokenType GetTokenType() const;
	std::string GetValue() const;

	int GetLine() const
	{
		return line;
	}

	void SetLine(int newLine)
	{
		line = newLine;
	}
};

#endif