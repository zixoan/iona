/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/VariableAssignNode.h"
#include "Visitor.h"

VariableAssignNode::VariableAssignNode(const std::string& name, const Ref<Node>& expression)
	: name(name), expression(expression)
{

}

void VariableAssignNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
