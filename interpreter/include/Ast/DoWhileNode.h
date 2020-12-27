/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef DO_WHILE_NODE_H
#define DO_WHILE_NODE_H

#include "Node.h"

class DoWhileNode : public Node, public std::enable_shared_from_this<DoWhileNode>
{
private:
	Ref<Node> expression;
	Ref<Node> block;
public:
	DoWhileNode(const std::string& fileName, int line, const Ref<Node>& expression, const Ref<Node>& block);

	~DoWhileNode() = default;

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
