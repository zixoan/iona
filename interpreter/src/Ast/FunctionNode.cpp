/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/FunctionNode.h"
#include "Visitor.h"

FunctionNode::FunctionNode(const std::string& name, const Token& returnToken, Ref<Node> block, const std::vector<Ref<Node>>& parameters)
	: name(name), returnToken(returnToken), block(block), parameters(parameters)
{
}

void FunctionNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}