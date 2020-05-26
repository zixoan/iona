/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FOR_I_NODE_H
#define FOR_I_NODE_H

#include <string>
#include "Node.h"

class ForINode : public Node, public std::enable_shared_from_this<ForINode>
{
private:
	std::string variableName;
	int from;
	int to;
	Ref<Node> block;
public:
	ForINode(const char* line, const std::string& variableName, int from, int to, const Ref<Node>& block);

	~ForINode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetVariableName() const
	{
		return variableName;
	}

	int GetFrom() const
	{
		return from;
	}

	int GetTo() const
	{
		return to;
	}

	Ref<Node> GetBlock() const
	{
		return block;
	}
};

#endif
