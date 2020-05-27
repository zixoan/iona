/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>
#include <vector>
#include <any>

enum TokenType
{
	Function,
	Name,
	Call,
	For,
	In,
	Step,
	While,
	Do,
	If,
	Else,

	CurlyLeft,
	CurlyRight,

	SquareLeft,
	SquareRight,

	ParanLeft,
	ParanRight,

	Var,

	Int,
	Float,
	Bool,
	String,
	IntArray,
	BoolArray,
	FloatArray,
	StringArray,
	Array,

	Auto,
	Return,
	Assign,
	Comma,
	Semicolon,
	Colon,
	New,
	Point,
	ExclamationMark,

	Plus,
	PlusPlus,
	Minus,
	MinusMinus,
	Multiply,
	Divide,

	Equals,
	NotEquals,
	GreaterThan,
	LessThan,
	GreaterEqualThan,
	LessEqualThan,

	None
};

struct VariableType
{
	TokenType type;
	std::any value;
};

static bool IsVariableType(const TokenType& type)
{
	return type == TokenType::Int || type == TokenType::String || type == TokenType::Float || type == TokenType::Bool
		|| type == TokenType::StringArray || type == TokenType::IntArray || type == TokenType::BoolArray || type == TokenType::FloatArray;
}

static bool IsVariableArrayType(const TokenType& type)
{
	return type == TokenType::StringArray || type == TokenType::IntArray || 
			type == TokenType::BoolArray || type == TokenType::FloatArray;
}

namespace Helper
{
	static std::string ToString(const TokenType& type)
	{
		static const char* values[] = { "Function", "Name", "Call", "For", "In", "Step", "While", "Do", "If", "Else", "CurlyLeft", "CurlyRight", "SquareLeft", "SquareRight", "ParanLeft", "ParanRight", "Var", "Int", "Float", "Bool", "String", "IntArray", "BoolArray", "FloatArray", "StringArray", "Array", "Auto", "Return", "Assign", "Comma", "Semicolon", "Colon", "New", "Point", "ExclamationMark", "Plus", "PlusPlus", "Minus", "MinusMinus", "Multiply", "Divide", "Equals", "NotEquals", "GreaterThan", "LessThan", "GreaterEqualThan", "LessEqualThan", "None" };
		return values[static_cast<int>(type)];
	}

	static bool StartsWith(const std::string& s, const std::string& suffix)
	{
		return s.rfind(suffix, 0) == 0;
	}

	static bool EndsWith(const std::string& s, const std::string& suffix)
	{
		return s.size() >= suffix.size() && 0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix);
	}
}

#endif