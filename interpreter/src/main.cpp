/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include <Lexer.h>
#include <Parser.h>
#include <Interpreter.h>

int main(int argc, char const* argv[])
{
	Lexer lexer(R"(
		var b = false

		func Main()
		{
			b = 1 == 1
			WriteLine(b)
		}
	)");

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
