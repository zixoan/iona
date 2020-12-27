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
    std::string fileName;
	int line = 0;
public:
	Node() = default;
	explicit Node(const std::string& fileName, int line) : fileName(fileName), line(line) { }
	virtual ~Node() = default;

	virtual void Accept(const Ref<Visitor>& v) = 0;

	std::string GetFileName() const
    {
	    return fileName;
    }

	int GetLine() const
	{
		return line;
	}
};

#endif