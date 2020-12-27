/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "SemanticAnalyzer.h"
#include <chrono>
#include <VariableSymbol.h>
#include <FunctionSymbol.h>

SemanticAnalyzer::SemanticAnalyzer(const std::vector<std::string>& args, const Ref<Node>& astRoot)
    : SemanticAnalyzer(args, astRoot, std::make_shared<ScopedSymbolTable>("global", 1, nullptr))
{

}

SemanticAnalyzer::SemanticAnalyzer(const std::vector<std::string>& args, const Ref<Node>& astRoot, const Ref<ScopedSymbolTable>& scope)
    : Visitor(astRoot), currentScope(scope)
{
    this->RegisterBuiltInSymbols();
}

void SemanticAnalyzer::RegisterBuiltInSymbols()
{
    this->currentScope->Add(FunctionSymbol("WriteLine"));
    this->currentScope->Add(FunctionSymbol("ReadLine"));
    this->currentScope->Add(FunctionSymbol("ReadInt"));
    this->currentScope->Add(FunctionSymbol("ReadFloat"));
    this->currentScope->Add(FunctionSymbol("ToUpperCase"));
    this->currentScope->Add(FunctionSymbol("ToLowerCase"));
    this->currentScope->Add(FunctionSymbol("StartsWith"));
    this->currentScope->Add(FunctionSymbol("EndsWith"));
    this->currentScope->Add(FunctionSymbol("Contains"));
    this->currentScope->Add(FunctionSymbol("Split"));
    this->currentScope->Add(FunctionSymbol("Trim"));
    this->currentScope->Add(FunctionSymbol("Size"));
    this->currentScope->Add(FunctionSymbol("Empty"));
    this->currentScope->Add(FunctionSymbol("Random"));
    this->currentScope->Add(FunctionSymbol("Range"));
    this->currentScope->Add(FunctionSymbol("Reverse"));
    this->currentScope->Add(FunctionSymbol("ToString"));
    this->currentScope->Add(FunctionSymbol("Min"));
    this->currentScope->Add(FunctionSymbol("Max"));
    this->currentScope->Add(FunctionSymbol("FileExists"));
    this->currentScope->Add(FunctionSymbol("FileRead"));
    this->currentScope->Add(FunctionSymbol("FileWrite"));
    this->currentScope->Add(FunctionSymbol("FileCopy"));
    this->currentScope->Add(FunctionSymbol("FileReadLines"));
    this->currentScope->Add(FunctionSymbol("FileWriteLines"));
    this->currentScope->Add(FunctionSymbol("FileList"));

    this->currentScope->Add(VariableSymbol("PI"));
    this->currentScope->Add(VariableSymbol("INT_MIN"));
    this->currentScope->Add(VariableSymbol("INT_MAX"));
    this->currentScope->Add(VariableSymbol("FLOAT_MIN"));
    this->currentScope->Add(VariableSymbol("FLOAT_MAX"));
}

void SemanticAnalyzer::PushScope(const std::string& name)
{
    this->currentScope = std::make_shared<ScopedSymbolTable>(name, this->currentScope->GetLevel() + 1, this->currentScope);
}

void SemanticAnalyzer::PushScope()
{
    this->PushScope(std::to_string(this->currentScope->GetLevel() + 1));
}

void SemanticAnalyzer::PopScope()
{
    this->currentScope = this->currentScope->GetParent();
}

void SemanticAnalyzer::EnsureVariableDeclaration(const Ref<Node>& node, const std::string& name)
{
    std::optional<Symbol> symbol = this->currentScope->Find(name);
    if (!symbol)
    {
        Exit(node->GetFileName(), node->GetLine(), "Variable '%s' is not declared in this scope", name.c_str());
    }
}

void SemanticAnalyzer::EnsureFunctionDeclaration(const Ref<Node>& node, const std::string& name)
{
    auto symbol = this->currentScope->Find(name);
    if (!symbol)
    {
        Exit(node->GetFileName(), node->GetLine(), "Function '%s' is not declared", name.c_str());
    }
}

void SemanticAnalyzer::EnsureVariableUniqueness(const Ref<Node>& node, const std::string& name)
{
    std::optional<Symbol> symbol = this->currentScope->Find(name);
    if (symbol)
    {
        Exit(node->GetFileName(), node->GetLine(), "Variable '%s' is already declared in this scope", name.c_str());
    }
}

void SemanticAnalyzer::EnsureFunctionUniqueness(const Ref<Node>& node, const std::string& name)
{
    std::optional<Symbol> symbol = this->currentScope->Find(name);
    if (symbol)
    {
        Exit(node->GetFileName(), node->GetLine(), "Function '%s' is already declared in this scope", name.c_str());
    }
}

void SemanticAnalyzer::Analyze()
{
    auto start = std::chrono::high_resolution_clock::now();

    this->astRoot->Accept(shared_from_this());

    auto end = std::chrono::high_resolution_clock::now();

    IONA_LOG("\n\nSemantic analyzing took %llims (%llins)\n",
             std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
             std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

void SemanticAnalyzer::Visit(const Ref<MainNode>& n)
{
    for (const auto& globalVariable : n->GetGlobalVariables())
    {
        globalVariable->Accept(shared_from_this());
    }

    // Pre register all function symbols before visiting the function block,
    // so that all function names are known inside those blocks
    for (const auto& globalFunction : n->GetGlobalFunctions())
    {
        Ref<FunctionNode> fun = std::dynamic_pointer_cast<FunctionNode>(globalFunction);

        this->EnsureFunctionUniqueness(n, fun->GetName());

        this->currentScope->Add(FunctionSymbol(fun->GetName()));
    }

    for (const auto& globalFunction : n->GetGlobalFunctions())
    {
        globalFunction->Accept(shared_from_this());
    }
}

void SemanticAnalyzer::Visit(const Ref<VariableDeclarationAssignNode>& n)
{
    this->EnsureVariableUniqueness(n, n->GetName());

    this->currentScope->Add(VariableSymbol(n->GetName()));

    n->GetExpression()->Accept(shared_from_this());
}

void SemanticAnalyzer::Visit(const Ref<VariableArrayDeclarationAssignNode>& n)
{
    this->EnsureVariableUniqueness(n, n->GetName());

    this->currentScope->Add(VariableSymbol(n->GetName()));

    for (auto& value : n->GetValues())
    {
        value->Accept(shared_from_this());
    }
}

void SemanticAnalyzer::Visit(const Ref<FunctionNode>& n)
{
    this->PushScope(n->GetName());
    n->GetBlock()->Accept(shared_from_this());
    this->PopScope();
}

void SemanticAnalyzer::Visit(const Ref<FunctionCallNode>& n)
{
    this->EnsureFunctionDeclaration(n, n->GetName());

    for (auto& functionCall : n->GetParameters())
    {
        functionCall->Accept(shared_from_this());
    }
}

void SemanticAnalyzer::Visit(const Ref<ForEachNode>& n)
{
    this->EnsureVariableUniqueness(n, n->GetVariableName());

    this->PushScope();

    this->currentScope->Add(VariableSymbol(n->GetVariableName()));

    n->GetExpression()->Accept(shared_from_this());
    n->GetBlock()->Accept(shared_from_this());

    this->PopScope();
}

void SemanticAnalyzer::Visit(const Ref<ForINode>& n)
{
    this->EnsureVariableUniqueness(n, n->GetVariableName());

    this->PushScope();

    this->currentScope->Add(VariableSymbol(n->GetVariableName()));

    n->GetBlock()->Accept(shared_from_this());

    this->PopScope();
}

void SemanticAnalyzer::Visit(const Ref<BlockNode>& n)
{
    for (auto& statement : n->GetStatements())
    {
        statement->Accept(shared_from_this());
    }
}

void SemanticAnalyzer::Visit(const Ref<BinaryNode>& n)
{
    n->GetLeft()->Accept(shared_from_this());
    n->GetRight()->Accept(shared_from_this());
}

void SemanticAnalyzer::Visit(const Ref<BooleanNode>& n)
{
    n->GetLeft()->Accept(shared_from_this());
    n->GetRight()->Accept(shared_from_this());
}

void SemanticAnalyzer::Visit(const Ref<IfNode>& n)
{
    n->GetExpression()->Accept(shared_from_this());

    this->PushScope();
    n->GetTrueBlock()->Accept(shared_from_this());
    this->PopScope();

    for (const auto& [expression, block] : n->GetElseIfBlocks())
    {
        this->PushScope();
        block->Accept(shared_from_this());
        this->PopScope();
    }

    if (n->GetElseBlock() != nullptr)
    {
        this->PushScope();
        n->GetElseBlock()->Accept(shared_from_this());
        this->PopScope();
    }
}

void SemanticAnalyzer::Visit(const Ref<WhileNode>& n)
{
    this->PushScope();

    n->GetExpression()->Accept(shared_from_this());
    n->GetBlock()->Accept(shared_from_this());

    this->PopScope();
}

void SemanticAnalyzer::Visit(const Ref<DoWhileNode>& n)
{
    this->PushScope();

    n->GetExpression()->Accept(shared_from_this());
    n->GetBlock()->Accept(shared_from_this());

    this->PopScope();
}

void SemanticAnalyzer::Visit(const Ref<VariableUsageNode>& n)
{
    this->EnsureVariableDeclaration(n, n->GetName());
}

void SemanticAnalyzer::Visit(const Ref<VariableAssignNode>& n)
{
    this->EnsureVariableDeclaration(n, n->GetName());

    n->GetExpression()->Accept(shared_from_this());
}

void SemanticAnalyzer::Visit(const Ref<VariableArrayUsageNode>& n)
{
    this->EnsureVariableDeclaration(n, n->GetName());
}

void SemanticAnalyzer::Visit(const Ref<VariableArrayAssignNode>& n)
{
    this->EnsureVariableDeclaration(n, n->GetName());

    n->GetExpression()->Accept(shared_from_this());

    this->currentScope->Add(VariableSymbol(n->GetName()));
}

void SemanticAnalyzer::Visit(const Ref<VariableIncrementDecrementNode>& n)
{
    this->EnsureVariableDeclaration(n, n->GetName());
}

void SemanticAnalyzer::Visit(const Ref<VariableCompoundAssignNode>& n)
{
    this->EnsureVariableDeclaration(n, n->GetName());
}
