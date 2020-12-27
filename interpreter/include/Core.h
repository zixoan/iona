/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef CORE_H
#define CORE_H

#include <stdexcept>
#include <memory>
#include <utility>
#include <vector>
#include "Token.h"

#ifndef NDEBUG
#define IONA_LOG(...) printf(__VA_ARGS__)
#else
#define IONA_LOG
#endif

template<typename T>
using Ref = std::shared_ptr<T>;

template <typename ...Args>
static void Exit(std::string fileName, int line, const std::string& error, Args ...args)
{
    std::string prefix(std::move(fileName));
    prefix.append("(line ").append(std::to_string(line)).append("): ").append(error);

    const char* finalError = prefix.c_str();

    size_t size = std::snprintf(nullptr, 0, finalError, args...) + 1;
	std::vector<char> buffer(size);
	std::snprintf(&buffer[0], size, finalError, args...);

	// TODO: Improve
	throw std::runtime_error(&buffer[0]);
}

template <typename ...Args>
static void Exit(const Token& token, const std::string& error, Args ...args)
{
    Exit(token.GetFileName(), token.GetLine(), error, args...);
}

#endif