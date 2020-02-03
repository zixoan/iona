/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef NODE_H
#define NODE_H

#include <sstream>
#include "Core.h"

class Visitor;

class Node
{
protected:
	const char* line;
public:
	Node() = default;
	Node(const char* line) : line(line) { }
	virtual ~Node() = default;

	virtual void Accept(const Ref<Visitor>& v) = 0;

	const char* GetLine() const
	{
		return line;
	}
};

#endif