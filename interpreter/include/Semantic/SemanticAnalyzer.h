/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Visitor.h"
#include <functional>
#include <stack>
#include "Core.h"
#include "InterpreterScope.h"
#include "ScopedSymbolTable.h"
#include <FunctionRegistry.h>

class SemanticAnalyzer : public Visitor, public std::enable_shared_from_this<SemanticAnalyzer>
{
private:
    Ref<ScopedSymbolTable> currentScope;

    void RegisterBuiltInSymbols();

    void PushScope(const std::string& name);
    void PushScope();
    void PopScope();

    void EnsureVariableDeclaration(const Ref<Node>& node, const std::string& name);
    void EnsureFunctionDeclaration(const Ref<Node>& node, const std::string& name);
    void EnsureVariableUniqueness(const Ref<Node>& node, const std::string& name);
    void EnsureFunctionUniqueness(const Ref<Node>& node, const std::string& name);
public:
    SemanticAnalyzer(const std::vector<std::string>& args, const Ref<Node>& astRoot);
    SemanticAnalyzer(const std::vector<std::string>& args, const Ref<Node>& astRoot, const Ref<ScopedSymbolTable>& scope);
    ~SemanticAnalyzer() = default;

    void Analyze();

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
    void Visit(const Ref<VariableUsageNode>& n) override;
    void Visit(const Ref<VariableAssignNode>& n) override;
    void Visit(const Ref<VariableArrayUsageNode>& n) override;
    void Visit(const Ref<VariableArrayAssignNode>& n) override;
    void Visit(const Ref<VariableIncrementDecrementNode>& n) override;
    void Visit(const Ref<VariableCompoundAssignNode>& n) override;

    void Visit(const Ref<ReturnNode>& n) override {};
    void Visit(const Ref<StringNode>& n) override {};
    void Visit(const Ref<IntNode>& n) override {};
    void Visit(const Ref<FloatNode>& n) override {};
    void Visit(const Ref<BoolNode>& n) override {};
};

#endif
