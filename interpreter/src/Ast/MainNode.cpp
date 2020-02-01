/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Ast/MainNode.h"
#include "Visitor.h"

MainNode::MainNode(const std::vector<Ref<Node>>& globalVariables, const std::vector<Ref<Node>>& globalFunctions)
	: globalVariables(globalVariables), globalFunctions(globalFunctions)
{

}

void MainNode::Accept(const Ref<Visitor>& v)
{
	v->Visit(shared_from_this());
}