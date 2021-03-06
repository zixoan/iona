/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/FunctionCallNode.h"
#include "Visitor.h"

FunctionCallNode::FunctionCallNode(const std::string& fileName, int line, std::string name, std::vector<Ref<Node>> parameters)
	: Node(fileName, line), name(std::move(name)), parameters(std::move(parameters))
{

}

void FunctionCallNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
