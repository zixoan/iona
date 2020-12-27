/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "VariableArrayUsageNode.h"
#include "Visitor.h"

VariableArrayUsageNode::VariableArrayUsageNode(const std::string& fileName, int line, const std::string& name, unsigned int index)
	: Node(fileName, line), name(name), index(index)
{

}

void VariableArrayUsageNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
