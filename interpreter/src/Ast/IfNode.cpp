/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "IfNode.h"
#include "Visitor.h"

IfNode::IfNode(const std::string& fileName, int line, const Ref<Node>& expression, const Ref<Node>& trueBlock, const std::map<Ref<Node>, Ref<Node>>& elseIfBlocks, const Ref<Node>& elseBlock)
	: Node(fileName, line), expression(expression), trueBlock(trueBlock), elseIfBlocks(elseIfBlocks), elseBlock(elseBlock)
{
}

void IfNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
