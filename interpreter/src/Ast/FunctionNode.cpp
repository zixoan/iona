/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/FunctionNode.h"
#include "Visitor.h"

FunctionNode::FunctionNode(const std::string& fileName, int line, std::string name, Ref<Node> block, std::vector<std::string> parameters)
	: Node(fileName, line), name(std::move(name)), block(std::move(block)), parameters(std::move(parameters))
{
}

void FunctionNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}