/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/Literal/StringNode.h"
#include "Visitor.h"

StringNode::StringNode(const std::string& fileName, int line, std::string  value, std::vector<Ref<Node>> expressions)
	: Node(fileName, line), value(std::move(value)), expressions(std::move(expressions))
{
}

void StringNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}