/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/VariableIncrementDecrementNode.h"
#include "Visitor.h"

VariableIncrementDecrementNode::VariableIncrementDecrementNode(const char* line, const std::string& name, int value)
	: Node(line), name(name), value(value)
{

}

void VariableIncrementDecrementNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
