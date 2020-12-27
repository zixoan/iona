/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_USAGE_NODE_H
#define VARIABLE_USAGE_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"

class VariableUsageNode : public Node, public std::enable_shared_from_this<VariableUsageNode>
{
private:
	std::string name;
public:
	VariableUsageNode(const std::string& fileName, int line, const std::string& name);

	~VariableUsageNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}
};

#endif