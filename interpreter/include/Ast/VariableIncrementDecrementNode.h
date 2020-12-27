/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_INCREMENT_NODE_H
#define VARIABLE_INCREMENT_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"

class VariableIncrementDecrementNode : public Node, public std::enable_shared_from_this<VariableIncrementDecrementNode>
{
private:
	std::string name;
	int value;
public:
	VariableIncrementDecrementNode(const std::string& fileName, int line, std::string name, int value);

	~VariableIncrementDecrementNode() override = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	int GetValue() const
	{
		return value;
	}
};

#endif