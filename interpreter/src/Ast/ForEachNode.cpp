/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "ForEachNode.h"
#include "Visitor.h"

ForEachNode::ForEachNode(const std::string& fileName, int line, std::string variableName, const Ref<Node>& expression, const Ref<Node>& block)
	: Node(fileName, line), variableName(std::move(variableName)), expression(expression), block(block)
{
}

void ForEachNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}