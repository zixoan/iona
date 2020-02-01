/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "ForEachNode.h"
#include "Visitor.h"

ForEachNode::ForEachNode(const std::string& variableName, const std::string& arrayName, const Ref<Node>& block) 
	: variableName(variableName), arrayName(arrayName), block(block)
{
}

void ForEachNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}