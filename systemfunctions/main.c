#include <Windows.h>
#include <lmcons.h>
#include <wchar.h>
#include <VersionHelpers.h>
#include <initguid.h>
#include <KnownFolders.h>
#include <ShlObj.h>

#define BUFSIZE MAX_PATH	// max lenght of a system path 

/*
   this is used to tell the linker to include user32.lib
   why comment? can be a bit misleading but its for historical reasons.
*/
#pragma comment(lib, "user32.lib")  

int wmain(void)
{
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	wprintf(L"Your screen is %dx%d\n\n", x, y);

	wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
	wchar_t username[UNLEN + 1];  // UNLEN definex max lenght of username
	wchar_t buf[BUFSIZE];

	DWORD size = sizeof(computerName) / sizeof(computerName[0]);
	DWORD len = sizeof(username) / sizeof(wchar_t);

	int r = GetComputerNameW(computerName, &size);

	if (!r)
	{
		wprintf(L"GetComputerName failed with %ld error\n", GetLastError());
		return -1;
	}

	wprintf(L"Computer name: %ls\n", computerName);   //%ls for wide character strings

	r = GetUserNameW(username, &len);

	if (!r)
	{
		wprintf(L"GetUserNameW failed with %ld error\n", GetLastError());
		return -2;
	}

	wprintf(L"Username: %ls\n", username);

	r = GetCurrentDirectoryW(BUFSIZE, buf);

	if (!r)
	{
		wprintf(L"GetDirectoryW failed with %ld error\n", GetLastError());
		return -3;
	}

	if (r > BUFSIZE)
	{
		wprintf(L"Buffer too small; needs %d characters\n", r);
		return -4;
	}

	wprintf(L"\nCurrent dir: %ls\n", buf);  // in this case its returning the folder where the .c file is located

	PWSTR path = NULL;

	//  known folders uses a set of GUID (Globally unique identifier) values for referencing important folders
	HRESULT hr = SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &path);

	if (SUCCEEDED(hr)) {
		wprintf(L"Your Documents folder is located at %ls\n", path);
	}

	CoTaskMemFree(path);

	wchar_t LogicalDrives[MAX_PATH] = { 0 };

	DWORD res = GetLogicalDriveStringsW(MAX_PATH, LogicalDrives);

	if (!res)
	{
		wprintf(L"GetLogicalDriveStringsW failed with %ld error\n", GetLastError());
		return -5;
	}

	if (res > 0 && res <= MAX_PATH)
	{
		wchar_t* SingleDrive = LogicalDrives;

		while (*SingleDrive) {
			wprintf(L"%ls\n", SingleDrive);

			SingleDrive += wcslen(SingleDrive) + 1;
		}
	}

	/*
	if (IsWindows10OrGreater()) {

	   wprintf(L"This is Windows 10+");
	}
	if (IsWindows8Point1OrGreater()) {
		wprintf(L"This is Windows 8.1+\n");
	}
	else if (IsWindows8OrGreater()) {
		wprintf(L"This is Windows 8\n");
	}
	else if (IsWindows7OrGreater()) {
		wprintf(L"This is Windows 7\n");
	}
	else if (IsWindowsVistaOrGreater()) {
		wprintf(L"This is Windows Vista\n");
	}
	else if (IsWindowsXPOrGreater()) {
		wprintf(L"This is Windows XP\n");
	}

		using this, this was returning my windows version was windows 8. so i found out that these functions are "deprecated"
		not exactly deprecated, but they may return innacurate values if using a win version that was released after these functions
		were introduced (my case)

		so, below is the "correct" approach:
	*/

	/*OSVERSIONINFOEX os_v;  // structure used to store version info about the os
	ZeroMemory(&os_v, sizeof(os_v));  //  cleaning any possible garbage values left

	os_v.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  // this indicates the size of the structure that will be passed. this may
														 // vary depending on the windows version, thats why its necessary

	if (GetVersionEx((LPOSVERSIONINFO)&os_v)) {
		if (os_v.dwMajorVersion == 10 && os_v.dwMinorVersion == 0) {
			wprintf(L"This is Windows 10\n");
		}
		else if (os_v.dwMajorVersion == 11) {
			wprintf(L"This is Windows 11\n");
		}
		else {
			wprintf(L"This is an unknown version of Windows\n");
		}
	}
	else {
		wprintf(L"Failed to retrieve Windows version information\n");
	}

		so i found out while trying to compile that this is not a valid approach as well, since this is the real deprecated way of doing
		it. it should be used the iswinxorgreater stuff
		but theres not a iswin11orgreater yet, i found out an implementation on github that tries to solve this. so im leaving this
		as is for now as an open question
		in the tutorial im following for this, its saying that there wasnt iswindows10orgreater, and nowadays it has,
		so this may be fixed in the near future

	*/

	MEMORYSTATUSEX m = { 0 };
	m.dwLength = sizeof(m);

	r = GlobalMemoryStatusEx(&m);

	if (!r)
	{
		wprintf(L"GlobalMemoryStatusEx failed with %ld error\n", r);
		return -6;
	}

	wprintf(L"\nMemory in use: %ld%%\n", m.dwMemoryLoad);
	wprintf(L"Total physical memory: %lld\n", m.ullTotalPhys);
	wprintf(L"Free physical memory: %lld\n", m.ullAvailPhys);
	wprintf(L"Total virtual memory: %lld\n", m.ullTotalVirtual);
	wprintf(L"Free virtual memory: %lld\n\n", m.ullAvailVirtual);

	unsigned __int64 freeCall, total, free;

	r = GetDiskFreeSpaceExW(L"C:\\", (PULARGE_INTEGER)&freeCall, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);

	if (!r)
	{
		wprintf(L"Failed to get free space disk with error %ld\n", GetLastError());
		return -7;
	}

	wprintf(L"Available space to caller: %I64u MB\n", freeCall / (1024*1024));
	wprintf(L"Total space: %I64u MB\n", total / (1024*1024));
	wprintf(L"Free space on drive: %I64u MB\n", free / (1024*1024));

	DWORD buf_size, mhz = MAX_PATH;
	HKEY key;

	long result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key);

	if (result != ERROR_SUCCESS)
	{
		wprintf(L"RegOpenKeyExW failed with %ld error\n", GetLastError());
		return -8;
	}

	result = RegQueryValueExW(key, L"~MHz", NULL, NULL, (LPBYTE)&mhz, &buf_size);

	if (result != ERROR_SUCCESS)
	{
		wprintf(L"RegQueryValueExW failed with %ld error\n", GetLastError());
		return -9;
	}

	wprintf(L"CPU speed: %lu MHz\n", mhz);

	result = RegCloseKey(key);

	if (result != ERROR_SUCCESS)
	{
		wprintf(L"RegCloseKey failed with %ld error\n", GetLastError());
		return -10;
	}

	wprintf(L"\nLocking screen..\n");
	Sleep(4000);

	r = LockWorkStation();
	if (!r)
	{
		wprintf(L"LockWorkStation failed with %ld error\n", GetLastError());
		return -1;
	}

	return 0;
}