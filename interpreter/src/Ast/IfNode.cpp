/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "IfNode.h"
#include "Visitor.h"

IfNode::IfNode(const Ref<Node>& expression, const Ref<Node>& trueBlock, const Ref<Node>& falseBlock) 
	: expression(expression), trueBlock(trueBlock), falseBlock(falseBlock)
{
}

void IfNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
