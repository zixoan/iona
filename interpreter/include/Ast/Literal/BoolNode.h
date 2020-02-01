/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef BOOL_NODE_H
#define BOOL_NODE_H

#include "Node.h"

class BoolNode : public Node, public std::enable_shared_from_this<BoolNode>
{
private:
	bool value;
public:
	BoolNode(const std::string& value);
	~BoolNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	bool GetValue() const
	{
		return value;
	}
};

#endif
