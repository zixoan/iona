/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <iostream>
#include <vector>
#include "Ast/Node.h"
#include "Lexer.h"

class Parser
{
private:
	Lexer& lexer;
	Token currentToken;

	void Advance(TokenType tokenType);

	Ref<Node> ParseMainFile();
	Ref<Node> ParseGlobalVariables();
	Ref<Node> ParseFunction();
	Ref<Node> ParseFunctionCall();
	Ref<Node> ParseFor();
	Ref<Node> ParseIf();

	std::vector<Ref<Node>> ParseFunctionStatements();
	Ref<Node> ParseBlock();

	Ref<Node> Factor();
	Ref<Node> Term();
	Ref<Node> Statement();
	Ref<Node> Expression();
public:
	const std::string MainFunctionName = "Main";

	Parser(Lexer& lexer);
	~Parser() = default;

	Ref<Node> Parse();
};

#endif