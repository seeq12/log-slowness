#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main()
{
	TCHAR szCurrentDirectory[MAX_PATH];
	TCHAR szSrcLogFilePaths[2][MAX_PATH];
	TCHAR szTestFilePath[MAX_PATH];
	DWORD cchCurrentDirectory = GetCurrentDirectory(_countof(szCurrentDirectory), szCurrentDirectory);
	DWORD retCode = 1;
	DWORD dwPos;
	HANDLE hAppend = INVALID_HANDLE_VALUE;
	LPCSTR szAppend = "Append line\n";
	DWORD dwBytesWritten;
	WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
	DWORD start, stop;

	_tcscpy_s(szSrcLogFilePaths[0], _countof(szSrcLogFilePaths[0]), szCurrentDirectory);
	_tcscat_s(szSrcLogFilePaths[0], _countof(szSrcLogFilePaths[0]), _T("\\fast.log"));

	_tcscpy_s(szSrcLogFilePaths[1], _countof(szSrcLogFilePaths[1]), szCurrentDirectory);
	_tcscat_s(szSrcLogFilePaths[1], _countof(szSrcLogFilePaths[1]), _T("\\slow.log"));

	for (int i = 0; i < 2; i++) {
		_tcscpy_s(szTestFilePath, _countof(szTestFilePath), szSrcLogFilePaths[i]);
		_tcscat_s(szTestFilePath, _countof(szTestFilePath), _T(".test"));

		if (CopyFile(szSrcLogFilePaths[i], szTestFilePath, FALSE) == FALSE) {
			_tprintf(_T("ERROR: Could not copy '%s' to '%s'"), szSrcLogFilePaths[i], szTestFilePath);
			goto error;
		}

		hAppend = CreateFile(szTestFilePath,
			FILE_APPEND_DATA,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hAppend == INVALID_HANDLE_VALUE) {
			_tprintf(_T("ERROR: Could not open '%s' for append"), szTestFilePath);
			goto error;
		}

		start = GetTickCount();
		for (int j = 0; j < 20; j++) {
			GetFileAttributesEx(szTestFilePath, GetFileExInfoStandard, &fileAttributeData);

			dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);

			WriteFile(hAppend, (BYTE*)szAppend, strlen(szAppend), &dwBytesWritten, NULL);
		}
		stop = GetTickCount();

		_tprintf(_T("%s -> %6d ms -> %d bytes\n"), szTestFilePath, stop - start, fileAttributeData.nFileSizeLow);

		CloseHandle(hAppend);
		hAppend = INVALID_HANDLE_VALUE;

		if (DeleteFile(szTestFilePath) == FALSE) {
			_tprintf(_T("ERROR: Could not delete '%s'"), szTestFilePath);
			goto error;
		}
	}

	retCode = 0;

error:
	if (hAppend != INVALID_HANDLE_VALUE) {
		CloseHandle(hAppend);
	}

	return retCode;
}