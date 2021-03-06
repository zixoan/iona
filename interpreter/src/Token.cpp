/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Token.h"

Token::Token() 
	: tokenType(TokenType::None), line(0)
{

}

Token::Token(const TokenType& tokenType, std::string  value, std::string fileName, const int line)
	: tokenType(tokenType), value(std::move(value)), fileName(std::move(fileName)), line(line)
{
}

TokenType Token::GetTokenType() const
{
	return this->tokenType;
}

std::string Token::GetValue() const
{
	return this->value;
}