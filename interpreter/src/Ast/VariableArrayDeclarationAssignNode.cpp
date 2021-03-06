/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "VariableArrayDeclarationAssignNode.h"
#include "Visitor.h"

VariableArrayDeclarationAssignNode::VariableArrayDeclarationAssignNode(const std::string& fileName,
                                                                       int line, std::string name,
                                                                       const TokenType arrayType,
                                                                       std::vector<Ref<Node>> values)
    : Node(fileName, line), name(std::move(name)), arrayType(arrayType), values(std::move(values))
{
}

void VariableArrayDeclarationAssignNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}