/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/Literal/FloatNode.h"
#include "Visitor.h"

FloatNode::FloatNode(const std::string& value)
	: value(std::stof(value))
{
}

void FloatNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}