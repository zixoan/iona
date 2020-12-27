/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/VariableIncrementDecrementNode.h"
#include "Visitor.h"

VariableIncrementDecrementNode::VariableIncrementDecrementNode(const std::string& fileName, int line, std::string name, int value)
	: Node(fileName, line), name(std::move(name)), value(value)
{

}

void VariableIncrementDecrementNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
