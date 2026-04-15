/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
*
* This software is available to you under a choice of one of two
* licenses.  You may choose to be licensed under the terms of the GNU
* General Public License (GPL) Version 2, available from the file
* COPYING in the main directory of this source tree, or the
* BSD license below:
*
*     Redistribution and use in source and binary forms, with or
*     without modification, are permitted provided that the following
*     conditions are met:
*
*      - Redistributions of source code must retain the above
*        copyright notice, this list of conditions and the following
*        disclaimer.
*
*      - Redistributions in binary form must reproduce the above
*        copyright notice, this list of conditions and the following
*        disclaimer in the documentation and/or other materials
*        provided with the distribution.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


#include <windows.h>
#include <assert.h>


#define ND_SHARE_ATTR (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE)

static inline DWORD nd_file_attributes(const wchar_t* path)
{
	if (path && path[0])
		return GetFileAttributesW(path);

	return 0;
}

int nd_file_exists(const wchar_t* path)
{
	return nd_file_attributes(path) != INVALID_FILE_ATTRIBUTES;
}

int nd_is_directory(const wchar_t* path)
{
	return nd_file_exists(path) &&
		(nd_file_attributes(path) & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int nd_is_same_file(const wchar_t* path1, const wchar_t* path2)
{
	if (!path1 || !path1[0] || !path2 || !path2[0])
		return 0;

	if (!lstrcmpiW(path1, path2))
		return 1;

	int is_same = 1;

	if (nd_file_exists(path1) && nd_file_exists(path2)) {
		if (nd_is_directory(path1) ^ nd_is_directory(path2))
			return 0; // if one file is directory & another - not - return false

		HANDLE f1 = CreateFileW(path1, FILE_READ_ATTRIBUTES,
					ND_SHARE_ATTR,
					0, OPEN_EXISTING,
					nd_is_directory(path1) ? FILE_FLAG_BACKUP_SEMANTICS : FILE_ATTRIBUTE_NORMAL, 0);
		if (f1 && f1 != INVALID_HANDLE_VALUE) {
			HANDLE f2 = CreateFileW(path2, FILE_READ_ATTRIBUTES,
						ND_SHARE_ATTR,
						0, OPEN_EXISTING,
						nd_is_directory(path2) ? FILE_FLAG_BACKUP_SEMANTICS : FILE_ATTRIBUTE_NORMAL, 0);
			if (f2 && f2 != INVALID_HANDLE_VALUE) {
				BY_HANDLE_FILE_INFORMATION info1;
				BY_HANDLE_FILE_INFORMATION info2;

				if (GetFileInformationByHandle(f1, &info1) &&
				    GetFileInformationByHandle(f2, &info2)) {
					if (info1.nFileIndexHigh == info2.nFileIndexHigh &&
					    info1.nFileIndexLow == info2.nFileIndexLow &&
					    info1.dwVolumeSerialNumber == info2.dwVolumeSerialNumber)
						is_same = 1;
				}
				CloseHandle(f2);
			}
			CloseHandle(f1);
		}
	}

	return is_same;
}

const wchar_t *nd_filename(const wchar_t *path)
{
	assert(path);
	int i;

	for (i = lstrlenW(path) - 2; i >= 0; i--)
		if (path[i] == '/' || path[i] == '\\')
			return &path[i] + 1;
	return path;
}
