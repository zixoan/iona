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
#include "Parser.h"
#include "InterpreterScope.h"
#include <FunctionRegistry.h>

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter>
{
private:
	Parser parser;
	Ref<Node> root;

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
	Interpreter(const std::vector<std::string>& args, const Parser& parser);
	Interpreter(const std::vector<std::string>& args, const Parser& parser, const Ref<InterpreterScope>& scope);
	~Interpreter() = default;

	void Interpret();

	VariableType GetCurrentVariable() const
	{
		return currentVariable;
	}

	void Visit(const Ref<MainNode>& n);
	void Visit(const Ref<VariableDeclarationAssignNode>& n);
	void Visit(const Ref<VariableArrayDeclarationAssignNode>& n);
	void Visit(const Ref<FunctionNode>& n);
	void Visit(const Ref<FunctionCallNode>& n);
	void Visit(const Ref<ForEachNode>& n);
	void Visit(const Ref<ForINode>& n);
	void Visit(const Ref<BlockNode>& n);
	void Visit(const Ref<BinaryNode>& n);
	void Visit(const Ref<BooleanNode>& n);
	void Visit(const Ref<IfNode>& n);
	void Visit(const Ref<WhileNode>& n);
	void Visit(const Ref<DoWhileNode>& n);
	void Visit(const Ref<StringNode>& n);
	void Visit(const Ref<IntNode>& n);
	void Visit(const Ref<FloatNode>& n);
	void Visit(const Ref<VariableUsageNode>& n);
	void Visit(const Ref<VariableAssignNode>& n);
	void Visit(const Ref<VariableArrayUsageNode>& n);
	void Visit(const Ref<VariableArrayAssignNode>& n);
	void Visit(const Ref<VariableIncrementDecrementNode>& n);
	void Visit(const Ref<VariableCompoundAssignNode>& n);
	void Visit(const Ref<BoolNode>& n);
	void Visit(const Ref<ReturnNode>& n);
};

#endif