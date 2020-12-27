/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "ForINode.h"
#include "Visitor.h"

ForINode::ForINode(const std::string& fileName, int line, const std::string& variableName, int from, int to, int step, const Ref<Node>& block)
	: Node(fileName, line), variableName(variableName), from(from), to(to), step(step), block(block)
{
}

void ForINode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}