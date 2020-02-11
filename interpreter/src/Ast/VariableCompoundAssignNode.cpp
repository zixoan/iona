/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/VariableCompoundAssignNode.h"
#include "Visitor.h"

VariableCompoundAssignNode::VariableCompoundAssignNode(const char* line, const std::string& name, const Ref<Node>& expression, const TokenType& operation)
	: Node(line), name(name), expression(expression), operation(operation)
{

}

void VariableCompoundAssignNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}