/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef CORE_H
#define CORE_H

#include <stdexcept>

#ifndef NDEBUG
#define IONA_LOG(...) printf(__VA_ARGS__)
#else
#define IONA_LOG
#endif

template<typename T>
using Ref = std::shared_ptr<T>;

template <typename ...Args>
static void Exit(const char* error, Args ...args)
{
	size_t size = std::snprintf(nullptr, 0, error, args...) + 1;
	std::vector<char> buffer(size);
	std::snprintf(&buffer[0], size, error, args...);

	// TODO: Improve
	throw std::runtime_error(&buffer[0]);
}

#endif