/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/VariableUsageNode.h"
#include "Visitor.h"

VariableUsageNode::VariableUsageNode(const std::string& fileName, int line, std::string name)
	: Node(fileName, line), name(std::move(name))
{

}

void VariableUsageNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
