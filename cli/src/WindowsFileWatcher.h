/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#ifndef WINDOWS_FILE_WATCHER_H
#define WINDOWS_FILE_WATCHER_H

#include "FileWatcher.h"
#include <string>

class WindowsFileWatcher : public FileWatcher
{
public:
	explicit WindowsFileWatcher(const char* path);

	void WatchDirectory() override;
};

#endif
