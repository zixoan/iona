/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FUNCTION_CALL_NODE_H
#define FUNCTION_CALL_NODE_H

#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

class FunctionCallNode : public Node, public std::enable_shared_from_this<FunctionCallNode>
{
private:
	std::string name;
	std::vector<Ref<Node>> parameters;
public:
	FunctionCallNode(const std::string& fileName, int line, const std::string& name, const std::vector<Ref<Node>>& parameters);

	~FunctionCallNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	std::vector<Ref<Node>> GetParameters()
	{
		return parameters;
	}
};

#endif