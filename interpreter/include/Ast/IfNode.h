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

class IfNode : public Node, public std::enable_shared_from_this<IfNode>
{
private:
	Ref<Node> expression;
	Ref<Node> trueBlock;
	Ref<Node> falseBlock;
public:
	IfNode(const Ref<Node>& expression, const Ref<Node>& trueBlock, const Ref<Node>& falseBlock);

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

	Ref<Node> GetFalseBlock() const
	{
		return falseBlock;
	}
};

#endif
