/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "WhileNode.h"
#include "Visitor.h"

WhileNode::WhileNode(const std::string& fileName, int line, const Ref<Node>& expression, const Ref<Node>& block)
	: Node(fileName, line), expression(expression), block(block)
{
}

void WhileNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
