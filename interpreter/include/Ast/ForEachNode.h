/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FOR_EACH_NODE_H
#define FOR_EACH_NODE_H

#include <string>
#include "Node.h"
#include "Token.h"

class ForEachNode : public Node, public std::enable_shared_from_this<ForEachNode>
{
private:
	std::string variableName;
	Ref<Node> expression;
	Ref<Node> block;
public:
	ForEachNode(const std::string& fileName, int line, const std::string& variableName, const Ref<Node>& expression, const Ref<Node>& block);

	~ForEachNode() = default;

	void Accept(const Ref<Visitor>& v) override;

	std::string GetVariableName() const
	{
		return variableName;
	}

	Ref<Node> GetExpression() const
	{
		return expression;
	}

	Ref<Node> GetBlock() const
	{
		return block;
	}
};

#endif
