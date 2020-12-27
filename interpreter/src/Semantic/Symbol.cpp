/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "Symbol.h"

Symbol::Symbol(std::string  name)
    : name(std::move(name))
{

}

std::string Symbol::GetName() const
{
    return this->name;
}
