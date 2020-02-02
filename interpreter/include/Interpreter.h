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

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter>
{
private:
	Parser parser;
	Ref<Node> root;

	std::vector<Ref<InterpreterScope>> scopes;
	std::map<std::string, Ref<FunctionNode>> globalFunctions;
	std::map<std::string, std::function<void(const std::stack<VariableType>& in, VariableType& out)>> internalFunctions;

	VariableType currentVariable;
	std::vector<Ref<Node>> currentFunctionCallFunctionParams;
	std::vector<Ref<Node>> currentFunctionCallParams;

	void RegisterInternalFunctions();
	Ref<InterpreterScope> FindScopeOfVariable(const std::string& variableName);
public:
	Interpreter(const Parser& parser);
	~Interpreter() = default;

	void Interpret();

	void Visit(const Ref<MainNode>& n);
	void Visit(const Ref<VariableDeclarationAssignNode>& n);
	void Visit(const Ref<VariableArrayDeclarationAssignNode>& n);
	void Visit(const Ref<FunctionNode>& n);
	void Visit(const Ref<FunctionCallNode>& n);
	void Visit(const Ref<ForEachNode>& n);
	void Visit(const Ref<BlockNode>& n);
	void Visit(const Ref<BinaryNode>& n);
	void Visit(const Ref<BooleanNode>& n);
	void Visit(const Ref<IfNode>& n);
	void Visit(const Ref<StringNode>& n);
	void Visit(const Ref<IntNode>& n);
	void Visit(const Ref<FloatNode>& n);
	void Visit(const Ref<VariableUsageNode>& n);
	void Visit(const Ref<VariableAssignNode>& n);
	void Visit(const Ref<VariableArrayUsageNode>& n);
	void Visit(const Ref<VariableArrayAssignNode>& n);
	void Visit(const Ref<BoolNode>& n);
};

#endif