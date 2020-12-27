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
	Ref<Node> expression;
public:
	VariableDeclarationAssignNode(const std::string& fileName, int line, const std::string& name, const Ref<Node>& expression);

	~VariableDeclarationAssignNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	Ref<Node> GetExpression() const
	{
		return expression;
	}
};

#endif