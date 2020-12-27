/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/VariableDeclarationAssignNode.h"
#include "Visitor.h"

VariableDeclarationAssignNode::VariableDeclarationAssignNode(const std::string& fileName, int line, const std::string& name, const Ref<Node>& expression)
	: Node(fileName, line), name(name), expression(expression)
{

}

void VariableDeclarationAssignNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}