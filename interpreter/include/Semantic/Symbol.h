/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

class Symbol
{
private:
    std::string name;
public:
    explicit Symbol(std::string name);

    std::string GetName() const;
};

#endif
