/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include "Symbol.h"

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(std::string name);
};

#endif
