/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef SCOPED_SYMBOL_TABLE_H
#define SCOPED_SYMBOL_TABLE_H

#include <map>
#include <optional>
#include "Core.h"
#include "Symbol.h"

class ScopedSymbolTable
{
private:
    std::string name;
    int level;
    Ref<ScopedSymbolTable> parent;
    std::map<std::string, Symbol> symbols;
public:
    ScopedSymbolTable(std::string name, int level, Ref<ScopedSymbolTable> parent);

    void Add(const Symbol& symbol);
    std::optional<Symbol> Find(const std::string& symbolName);

    int GetLevel() const;
    Ref<ScopedSymbolTable> GetParent() const;
};

#endif
