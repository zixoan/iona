/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef IF_NODE_H
#define IF_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"
#include <map>

class IfNode : public Node, public std::enable_shared_from_this<IfNode>
{
private:
	Ref<Node> expression;
	Ref<Node> trueBlock;
	std::map<Ref<Node>, Ref<Node>> elseIfBlocks;
	Ref<Node> elseBlock;
public:
	IfNode(const std::string& fileName, int line, const Ref<Node>& expression, const Ref<Node>& trueBlock, std::map<Ref<Node>, Ref<Node>> elseIfBlocks, const Ref<Node>& elseBlock);

	~IfNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	Ref<Node> GetExpression() const
	{
		return expression;
	}

	Ref<Node> GetTrueBlock() const
	{
		return trueBlock;
	}

	std::map<Ref<Node>, Ref<Node>> GetElseIfBlocks() const
	{
		return elseIfBlocks;
	}

	Ref<Node> GetElseBlock() const
	{
		return elseBlock;
	}
};

#endif
