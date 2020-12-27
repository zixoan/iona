/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Visitor.h"
#include <functional>
#include <stack>
#include "Core.h"
#include "InterpreterScope.h"
#include <FunctionRegistry.h>

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter>
{
private:
	std::vector<Ref<InterpreterScope>> scopes;
	std::map<std::string, Ref<FunctionNode>> globalFunctions;
	FunctionRegistry internalFunctions;

	VariableType currentVariable;
	std::vector<std::string> currentFunctionCallFunctionParams;
	std::vector<Ref<Node>> currentFunctionCallParams;

	void RegisterInternalFunctions();
	void RegisterInternalVariables();
	Ref<InterpreterScope> FindScopeOfVariable(const std::string& variableName);
public:
	Interpreter(const std::vector<std::string>& args, const Ref<Node>& astRoot);
	Interpreter(const std::vector<std::string>& args, const Ref<Node>& astRoot, const Ref<InterpreterScope>& scope);
	~Interpreter() = default;

	void Interpret();

	VariableType GetCurrentVariable() const
	{
		return currentVariable;
	}

	void Visit(const Ref<MainNode>& n) override;
	void Visit(const Ref<VariableDeclarationAssignNode>& n) override;
	void Visit(const Ref<VariableArrayDeclarationAssignNode>& n) override;
	void Visit(const Ref<FunctionNode>& n) override;
	void Visit(const Ref<FunctionCallNode>& n) override;
	void Visit(const Ref<ForEachNode>& n) override;
	void Visit(const Ref<ForINode>& n) override;
	void Visit(const Ref<BlockNode>& n) override;
	void Visit(const Ref<BinaryNode>& n) override;
	void Visit(const Ref<BooleanNode>& n) override;
	void Visit(const Ref<IfNode>& n) override;
	void Visit(const Ref<WhileNode>& n) override;
	void Visit(const Ref<DoWhileNode>& n) override;
	void Visit(const Ref<StringNode>& n) override;
	void Visit(const Ref<IntNode>& n) override;
	void Visit(const Ref<FloatNode>& n) override;
	void Visit(const Ref<VariableUsageNode>& n) override;
	void Visit(const Ref<VariableAssignNode>& n) override;
	void Visit(const Ref<VariableArrayUsageNode>& n) override;
	void Visit(const Ref<VariableArrayAssignNode>& n) override;
	void Visit(const Ref<VariableIncrementDecrementNode>& n) override;
	void Visit(const Ref<VariableCompoundAssignNode>& n) override;
	void Visit(const Ref<BoolNode>& n) override;
	void Visit(const Ref<ReturnNode>& n) override;
};

#endif