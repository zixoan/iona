/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "BinaryNode.h"
#include "Visitor.h"

BinaryNode::BinaryNode(const Ref<Node>& left, const TokenType& operant, const Ref<Node>& right) 
	: left(left), operant(operant), right(right)
{
}

void BinaryNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}