/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef STRING_NODE_H
#define STRING_NODE_H

#include <utility>
#include <vector>
#include "Node.h"

class StringNode : public Node, public std::enable_shared_from_this<StringNode>
{
private:
	std::string value;
	std::vector<Ref<Node>> expressions;
public:
	StringNode(const char* line, const std::string& value, const std::vector<Ref<Node>>& expressions);

	~StringNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetValue() const
	{
		return value;
	}

	std::vector<Ref<Node>> GetExpressions() const
	{
		return expressions;
	}
};

#endif
