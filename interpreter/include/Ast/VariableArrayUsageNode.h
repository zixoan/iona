/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_ARRAY_USAGE_NODE_H
#define VARIABLE_ARRAY_USAGE_NODE_H

#include <string>
#include "Node.h"

class VariableArrayUsageNode : public Node, public std::enable_shared_from_this<VariableArrayUsageNode>
{
private:
	std::string name;
	unsigned int index;
public:
	VariableArrayUsageNode(const std::string& name, unsigned int index);

	~VariableArrayUsageNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	unsigned int GetIndex() const
	{
		return index;
	}
};

#endif