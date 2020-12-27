/*
 * Copyright (c) 2020 Philip "zixoan" and individual contributors.
 * Subject to the GNU GPLv3 license. See LICENSE file for more information.
 *
 * SPDX-License-Identifier:	GPL-3.0-only
 */

#include "WindowsFileWatcher.h"
#include <Windows.h>
#include <sstream>

WindowsFileWatcher::WindowsFileWatcher(const char* path) : FileWatcher(path)
{

}

void WindowsFileWatcher::WatchDirectory()
{
	char buf[2048];
	DWORD nRet;
    char fileName[MAX_PATH];
	HANDLE file = CreateFile(this->path, GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                             nullptr);

	OVERLAPPED pollingOverlap;

	FILE_NOTIFY_INFORMATION* pNotify;
    pollingOverlap.OffsetHigh = 0;
	pollingOverlap.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	int notifications = 0;
	while (ReadDirectoryChangesW(file, &buf, sizeof(buf), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE, &nRet, &pollingOverlap, nullptr))
	{
		WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

		pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf);
		strcpy(fileName, "");
		WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, fileName, sizeof(fileName), nullptr, nullptr);
		fileName[pNotify->FileNameLength / 2] = '\0';

		if (stricmp(fileName, "Main.ion") == 0 || stricmp(fileName, "Main.iona") == 0)
		{
			notifications++;
		}

		if (notifications % 2)
		{
			printf("Rerunning..\n");

			std::stringstream command;
			command << "ionai \"" << this->path << fileName << "\"";
			system(command.str().c_str());
		}
	}

	CloseHandle(file);
}