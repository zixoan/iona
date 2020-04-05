/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H

class FileWatcher
{
protected:
	const char* path;
public:
	FileWatcher(const char* path) : path(path) { }
	virtual ~FileWatcher() = default;

	virtual void WatchDirectory() = 0;
};

#endif