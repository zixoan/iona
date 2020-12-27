/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include <fstream>
#include <filesystem>
#include <SemanticAnalyzer.h>
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Standard.h"

int main(int argc, char const* argv[])
{
	if (argc > 1)
	{
		if (!Helper::EndsWith(argv[1], ".ion") && !Helper::EndsWith(argv[1], ".iona"))
		{
			std::cout << "Source files for iona must end with '.ion' or '.iona' ('" << argv[1] << "')" << std::endl;
			return EXIT_SUCCESS;
		}

		if (!std::filesystem::exists(argv[1]))
		{
			std::cout << "Source file '" << argv[1] << "' does not exist" << std::endl;
			return EXIT_SUCCESS;
		}

		std::vector<std::string> args;
		args.reserve(argc);
		for (size_t i = 1; i < argc; i++)
		{
			args.emplace_back(argv[i]);
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

		try
		{
		    auto start = std::chrono::high_resolution_clock::now();

            Ref<Node> astRoot = parser.Parse();

            auto end = std::chrono::high_resolution_clock::now();

            IONA_LOG("\nParsing took %llims (%llins)\n\n",
                     std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
                     std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

            Ref<SemanticAnalyzer> semanticAnalyzer = std::make_shared<SemanticAnalyzer>(args, astRoot);

            semanticAnalyzer->Analyze();

            Ref<Interpreter> interpreter = std::make_shared<Interpreter>(args, astRoot);

			interpreter->Interpret();
			return EXIT_SUCCESS;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;

			return EXIT_FAILURE;
		}
	}
	else if (argc == 1)
	{
		std::cout << "Started iona in interactive mode" << std::endl;
		std::cout << "Start typing code:" << std::endl;

		std::vector<std::string> args;
		args.reserve(argc);
		for (size_t i = 1; i < argc; i++)
		{
			args.emplace_back(argv[i]);
		}


		Ref<ScopedSymbolTable> globalSemanticAnalyzerScope = std::make_shared<ScopedSymbolTable>("global", 0, nullptr);
		Ref<InterpreterScope> globalInterpreterScope = std::make_shared<InterpreterScope>();

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

			try
			{
				Lexer lexer(line, "console");
				Parser parser(lexer);

				auto statement = parser.Statement();

				Ref<SemanticAnalyzer> semanticAnalyzer = std::make_shared<SemanticAnalyzer>(args, statement, globalSemanticAnalyzerScope);
				semanticAnalyzer->Analyze();

				Ref<Interpreter> interpreter = std::make_shared<Interpreter>(args, statement, globalInterpreterScope);

				statement->Accept(interpreter);

				// Auto print variable statements
				if (IsVariableType(interpreter->GetCurrentVariable().type))
				{
					VariableType vt;
					std::vector<VariableType> inParams = { interpreter->GetCurrentVariable() };
					Iona::Console::WriteLine(inParams, vt);
				}
				continue;
			}
			catch (const std::exception& exception)
			{
				std::cout << exception.what() << std::endl;
				continue;
			}
		}
	}

	return EXIT_SUCCESS;
}
