/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H

#include "Symbol.h"

class FunctionSymbol : public Symbol
{
public:
    FunctionSymbol(std::string name);
};

#endif
