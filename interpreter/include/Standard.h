/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef STANDARD_H
#define STANDARD_H

#include <iostream>

namespace Console
{
	static void WriteLine(const std::stack<VariableType>& in, VariableType& out)
	{
		if (in.size() == 1)
		{
			VariableType v = in.top();

			switch (v.type)
			{
				case TokenType::String:
					printf("%s\n", std::any_cast<std::string>(v.value).c_str());
					break;
				case TokenType::Int:
					printf("%i\n", std::any_cast<int>(v.value));
					break;
				case TokenType::Float:
					printf("%g\n", std::any_cast<float>(v.value));
					break;
				case TokenType::Bool:
					printf("%s\n", std::any_cast<bool>(v.value) ? "true" : "false");
					break;
				// TODO: It hurts my eyes, improve this for arrays!!
				case TokenType::IntArray:
				{
					std::stringstream out;
					for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
					{
						out << std::any_cast<int>(variable.value) << ", ";
					}
					std::string s = out.str();
					printf("[%s]\n", s.substr(0, s.size() - 2).c_str());
					break;
				}
				case TokenType::FloatArray:
				{
					std::stringstream out;
					for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
					{
						out << std::any_cast<float>(variable.value) << ", ";
					}
					std::string s = out.str();
					printf("[%s]\n", s.substr(0, s.size() - 2).c_str());
					break;
				}
				case TokenType::StringArray:
				{
					std::stringstream out;
					for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
					{
						out << std::any_cast<std::string>(variable.value) << ", ";
					}
					std::string s = out.str();
					printf("[%s]\n", s.substr(0, s.size() - 2).c_str());
					break;
				}
				case TokenType::BoolArray:
				{
					std::stringstream out;
					for (auto& variable : std::any_cast<std::vector<VariableType>>(v.value))
					{
						out << (std::any_cast<bool>(variable.value) ? "true" : "false") << ", ";
					}
					std::string s = out.str();
					printf("[%s]\n", s.substr(0, s.size() - 2).c_str());
					break;
				}
				default:
					std::cerr << "Unsupported variable type " << Helper::ToString(v.type) << " for internal function WriteLine()" << std::endl;
			}
		}
	}

	static void ReadLine(const std::stack<VariableType>& in, VariableType& out)
	{
		std::string input;
		std::getline(std::cin, input);

		out.type = TokenType::String;
		out.value = input;
	}
}

#endif