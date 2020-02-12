/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "DoWhileNode.h"
#include "Visitor.h"

DoWhileNode::DoWhileNode(const char* line, const Ref<Node>& expression, const Ref<Node>& block)
	: Node(line), expression(expression), block(block)
{
}

void DoWhileNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
