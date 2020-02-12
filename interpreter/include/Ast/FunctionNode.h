/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FUNCTIO_NODE_H
#define FUNCTIO_NODE_H

#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

class FunctionNode : public Node, public std::enable_shared_from_this<FunctionNode>
{
private:
	std::string name;
	Ref<Node> block;
	std::vector<std::string> parameters;
public:
	FunctionNode(const std::string& name, Ref<Node> block, const std::vector<std::string>& parameters);

	~FunctionNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetName() const
	{
		return name;
	}

	Ref<Node> GetBlock()
	{
		return block;
	}

	std::vector<std::string> GetParameters()
	{
		return parameters;
	}
};

#endif