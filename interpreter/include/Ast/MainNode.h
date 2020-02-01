/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef MAIN_NODE_H
#define MAIN_NODE_H

#include <vector>
#include "Node.h"

class MainNode : public Node, public std::enable_shared_from_this<MainNode>
{
private:
	std::vector<Ref<Node>> globalVariables;
	std::vector<Ref<Node>> globalFunctions;
public:
	MainNode(const std::vector<Ref<Node>>& globalVariables, const std::vector<Ref<Node>>& globalFunctions);

	~MainNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::vector<Ref<Node>>& GetGlobalVariables()
	{
		return globalVariables;
	}

	std::vector<Ref<Node>>& GetGlobalFunctions()
	{
		return globalFunctions;
	}
};

#endif