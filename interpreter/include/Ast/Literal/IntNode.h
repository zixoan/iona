/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef INT_NODE_H
#define INT_NODE_H

#include <utility>
#include "Node.h"

class IntNode : public Node, public std::enable_shared_from_this<IntNode>
{
private:
	int value;
public:
	IntNode(const std::string& value);
	~IntNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	int GetValue() const
	{
		return value;
	}
};

#endif
