/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_DECLARATION_ASSIGN_NODE_H
#define VARIABLE_DECLARATION_ASSIGN_NODE_H

#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

class VariableDeclarationAssignNode : public Node, public std::enable_shared_from_this<VariableDeclarationAssignNode>
{
private:
	std::string name;
	Token valueToken;
public:
	VariableDeclarationAssignNode(const std::string& name, const Token& valueToken);

	~VariableDeclarationAssignNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	Token GetValueToken() const
	{
		return valueToken;
	}
};

#endif