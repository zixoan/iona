/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/Literal/IntNode.h"
#include "Visitor.h"

IntNode::IntNode(const std::string& value) 
	: value(std::stoi(value))
{
}

void IntNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}