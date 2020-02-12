/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VISITOR_H
#define VISITOR_H

#include "Ast/MainNode.h"
#include "Ast/VariableDeclarationAssignNode.h"
#include "Ast/FunctionNode.h"
#include "Ast/BlockNode.h"
#include "Ast/FunctionCallNode.h"
#include <Ast/Literal/StringNode.h>
#include <Ast/Literal/IntNode.h>
#include <Ast/VariableUsageNode.h>
#include <Ast/VariableAssignNode.h>
#include <Ast/Literal/FloatNode.h>
#include <Ast/Literal/BoolNode.h>
#include <Ast/BinaryNode.h>
#include <Ast/VariableArrayDeclarationAssignNode.h>
#include <Ast/VariableArrayUsageNode.h>
#include <Ast/ForEachNode.h>
#include <Ast/BooleanNode.h>
#include <Ast/VariableArrayAssignNode.h>
#include <Ast/IfNode.h>
#include <Ast/WhileNode.h>
#include <Ast/ReturnNode.h>
#include <Ast/VariableIncrementDecrementNode.h>
#include <Ast/VariableCompoundAssignNode.h>
#include <Ast/DoWhileNode.h>

class Visitor
{
public:
	virtual void Visit(const Ref<MainNode>& n) = 0;
	virtual void Visit(const Ref<VariableDeclarationAssignNode>& n) = 0;
	virtual void Visit(const Ref<VariableArrayDeclarationAssignNode>& n) = 0;
	virtual void Visit(const Ref<FunctionNode>& n) = 0;
	virtual void Visit(const Ref<FunctionCallNode>& n) = 0;
	virtual void Visit(const Ref<ForEachNode>& n) = 0;
	virtual void Visit(const Ref<BlockNode>& n) = 0;
	virtual void Visit(const Ref<BinaryNode>& n) = 0;
	virtual void Visit(const Ref<BooleanNode>& n) = 0;
	virtual void Visit(const Ref<IfNode>& n) = 0;
	virtual void Visit(const Ref<WhileNode>& n) = 0;
	virtual void Visit(const Ref<DoWhileNode>& n) = 0;
	virtual void Visit(const Ref<ReturnNode>& n) = 0;

	virtual void Visit(const Ref<StringNode>& n) = 0;
	virtual void Visit(const Ref<IntNode>& n) = 0;
	virtual void Visit(const Ref<FloatNode>& n) = 0;
	virtual void Visit(const Ref<BoolNode>& n) = 0;

	virtual void Visit(const Ref<VariableUsageNode>& n) = 0;
	virtual void Visit(const Ref<VariableAssignNode>& n) = 0;
	virtual void Visit(const Ref<VariableArrayUsageNode>& n) = 0;
	virtual void Visit(const Ref<VariableArrayAssignNode>& n) = 0;
	virtual void Visit(const Ref<VariableIncrementDecrementNode>& n) = 0;
	virtual void Visit(const Ref<VariableCompoundAssignNode>& n) = 0;
};

#endif