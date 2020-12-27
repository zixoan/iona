/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "ScopedSymbolTable.h"

ScopedSymbolTable::ScopedSymbolTable(std::string name, int level, Ref<ScopedSymbolTable> parent)
    : name(std::move(name)), level(level), parent(std::move(parent))
{

}

void ScopedSymbolTable::Add(const Symbol& symbol)
{
    this->symbols.insert(std::pair<std::string, Symbol>(symbol.GetName(), symbol));
}

std::optional<Symbol> ScopedSymbolTable::Find(const std::string& symbolName)
{
    auto iterator = this->symbols.find(symbolName);
    if (iterator == this->symbols.end())
    {
        if (this->parent == nullptr)
        {
            return std::nullopt;
        }

        // Try to recursively find the symbol in parent scopes
        return this->parent->Find(symbolName);
    }

    return std::make_optional(iterator->second);
}

int ScopedSymbolTable::GetLevel() const
{
    return this->level;
}

Ref<ScopedSymbolTable> ScopedSymbolTable::GetParent() const
{
    return this->parent;
}
