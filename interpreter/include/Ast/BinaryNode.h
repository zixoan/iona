/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef BINARY_NODE_H
#define BINARY_NODE_H

#include <vector>
#include "Node.h"
#include "Token.h"

class BinaryNode : public Node, public std::enable_shared_from_this<BinaryNode>
{
private:
	Ref<Node> left;
	TokenType operant;
	Ref<Node> right;
public:
	BinaryNode(const Ref<Node>& left, const TokenType& operant, const Ref<Node>& right);

	~BinaryNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	Ref<Node> GetLeft() const
	{
		return left;
	}

	TokenType GetOperant() const
	{
		return operant;
	}

	Ref<Node> GetRight() const
	{
		return right;
	}
};

#endif
