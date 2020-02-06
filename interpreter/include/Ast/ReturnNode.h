/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef RETURN_NODE_H
#define RETURN_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"

class ReturnNode : public Node, public std::enable_shared_from_this<ReturnNode>
{
private:
	Ref<Node> expression;
public:
	ReturnNode(const Ref<Node>& expression);

	~ReturnNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	Ref<Node> GetExpression() const
	{
		return expression;
	}
};

#endif