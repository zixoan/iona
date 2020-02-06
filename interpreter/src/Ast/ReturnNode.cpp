/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/ReturnNode.h"
#include "Visitor.h"

ReturnNode::ReturnNode(const Ref<Node>& expression)
	: expression(expression)
{

}

void ReturnNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}
