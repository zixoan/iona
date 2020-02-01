/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "BlockNode.h"
#include "Visitor.h"

BlockNode::BlockNode(const std::vector<Ref<Node>>& statements)
	: statements(statements)
{

}

void BlockNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
