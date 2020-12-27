/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/MainNode.h"
#include "Visitor.h"

MainNode::MainNode(std::vector<Ref<Node>> globalVariables, std::vector<Ref<Node>> globalFunctions)
	: globalVariables(std::move(globalVariables)), globalFunctions(std::move(globalFunctions))
{

}

void MainNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}