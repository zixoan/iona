/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "VariableArrayAssignNode.h"
#include "Visitor.h"

VariableArrayAssignNode::VariableArrayAssignNode(const char* line, const std::string& name, unsigned int index, const Ref<Node>& expression)
	: Node(line), name(name), index(index), expression(expression)
{
}

void VariableArrayAssignNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
