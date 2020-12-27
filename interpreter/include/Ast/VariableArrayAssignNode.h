/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_ARRAY_ASSIGN_NODE_H
#define VARIABLE_ARRAY_ASSIGN_NODE_H

#include <string>
#include "Node.h"

class VariableArrayAssignNode : public Node, public std::enable_shared_from_this<VariableArrayAssignNode>
{
private:
	std::string name;
	unsigned int index;
	Ref<Node> expression;
public:
	VariableArrayAssignNode(const std::string& fileName, int line, std::string name, unsigned int index, const Ref<Node>& expression);

	~VariableArrayAssignNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	unsigned int GetIndex() const
	{
		return index;
	}

	Ref<Node> GetExpression() const
	{
		return expression;
	}
};

#endif