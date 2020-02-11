/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef WHILE_NODE_H
#define WHILE_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"

class WhileNode : public Node, public std::enable_shared_from_this<WhileNode>
{
private:
	Ref<Node> expression;
	Ref<Node> block;
public:
	WhileNode(const char* line, const Ref<Node>& expression, const Ref<Node>& block);

	~WhileNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	Ref<Node> GetExpression() const
	{
		return expression;
	}

	Ref<Node> GetBlock() const
	{
		return block;
	}
};

#endif
