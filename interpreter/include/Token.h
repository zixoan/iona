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

	std::string line;
public:
	Token();
	Token(const TokenType& tokenType, const std::string& value, const std::string& line = "");
	~Token() = default;

	TokenType GetTokenType() const;
	std::string GetValue() const;

	const char* GetLine() const
	{
		return line.c_str();
	}

	void SetLine(const std::string& line)
	{
		this->line = line;
	}
};

#endif