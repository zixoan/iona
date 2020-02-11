/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_ASSIGN_NODE_H
#define VARIABLE_ASSIGN_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"

class VariableAssignNode : public Node, public std::enable_shared_from_this<VariableAssignNode>
{
private:
	std::string name;
	Ref<Node> expression;
public:
	VariableAssignNode(const char* line, const std::string& name, const Ref<Node>& expression);

	~VariableAssignNode() = default;

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