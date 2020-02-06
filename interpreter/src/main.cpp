/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include <fstream>
#include <filesystem>
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

bool EndsWith(const std::string& s, const std::string& suffix)
{
	return s.size() >= suffix.size() && 0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix);
}

int main(int argc, char const* argv[])
{
	if (argc == 2)
	{
		if (!EndsWith(argv[1], ".ion"))
		{
			std::cout << "Source files for iona must end with '.ion' ('" << argv[1] << "')" << std::endl;
			return EXIT_SUCCESS;
		}

		if (!std::filesystem::exists(argv[1]))
		{
			std::cout << "Source file '" << argv[1] << "' does not exist" << std::endl;
			return EXIT_SUCCESS;
		}

		std::ifstream in(argv[1]);
		std::string source;
		std::string line;
		while (std::getline(in, line))
		{
			source.append(line).append("\n");
		}

		Lexer lexer(source, "Main.ion");
		Parser parser(lexer);
		Ref<Interpreter> interpreter = std::make_shared<Interpreter>(parser);

		try
		{
			interpreter->Interpret();
			return EXIT_SUCCESS;
		}
		catch (...)
		{
			return EXIT_FAILURE;
		}
	}
	else if (argc == 1)
	{
		std::cout << "Started iona in interactive mode" << std::endl;
		std::cout << "Start typing code:" << std::endl;

		Ref<InterpreterScope> globalScope = std::make_shared<InterpreterScope>();

		while (true)
		{
			std::cout << ">> ";
			std::string line;
			std::getline(std::cin, line);

			if (line.empty())
			{
				continue;
			}

			if (line == "exit")
			{
				break;
			}
			
			Lexer lexer(line, "console");
			Parser parser(lexer);

			try
			{
				auto statement = parser.Statement();

				Ref<Interpreter> interpreter = std::make_shared<Interpreter>(parser, globalScope);

				statement->Accept(interpreter);
				continue;
			}
			catch (...)
			{
				std::cout << std::endl;
				continue;
			}
		}
	}

	return EXIT_SUCCESS;
}
