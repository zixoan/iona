/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/Literal/StringNode.h"
#include "Visitor.h"

StringNode::StringNode(const char* line, const std::string& value)
	: Node(line), value(value)
{
}

void StringNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}