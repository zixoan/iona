/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/FunctionNode.h"
#include "Visitor.h"

FunctionNode::FunctionNode(const std::string& name, Ref<Node> block, const std::vector<std::string>& parameters)
	: name(name), block(block), parameters(parameters)
{
}

void FunctionNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}