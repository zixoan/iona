/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

class BlockNode : public Node, public std::enable_shared_from_this<BlockNode>
{
private:
	std::vector<Ref<Node>> statements;
public:
	BlockNode(const std::vector<Ref<Node>>& statements);

	~BlockNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::vector<Ref<Node>> GetStatements()
	{
		return statements;
	}
};

#endif
