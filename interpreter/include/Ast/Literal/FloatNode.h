/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FLOAT_NODE_H
#define FLOAT_NODE_H

#include "Node.h"

class FloatNode : public Node, public std::enable_shared_from_this<FloatNode>
{
private:
	float value;
public:
	explicit FloatNode(const std::string& value);
	~FloatNode() override = default;

	void Accept(const Ref<Visitor>& v) override;

	float GetValue() const
	{
		return value;
	}
};

#endif
