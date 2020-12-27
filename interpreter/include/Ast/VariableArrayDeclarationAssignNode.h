/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_ARRAY_DECLARATION_ASSIGN_NODE_H
#define VARIABLE_ARRAY_DECLARATION_ASSIGN_NODE_H

#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

class VariableArrayDeclarationAssignNode : public Node, public std::enable_shared_from_this<VariableArrayDeclarationAssignNode>
{
private:
	std::string name;
	TokenType arrayType;
	std::vector<Ref<Node>> values;
public:
	VariableArrayDeclarationAssignNode(std::string name, const TokenType arrayType, std::vector<Ref<Node>> values);

	~VariableArrayDeclarationAssignNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	TokenType GetArrayType() const
	{
		return arrayType;
	}

	std::vector<Ref<Node>> GetValues() const
	{
		return values;
	}
};

#endif