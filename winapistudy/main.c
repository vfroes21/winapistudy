#include <Windows.h>
#include <wchar.h>

/* interesting note :
   wchar.h is needed for wprintf. i thought it wasnt necessary. but in the end i figured out that stdio.h also implements wprintf
*/


// with wmain, executions begins at wmainCRTStartup
int wmain(int argc, wchar_t** argv) {
	PDWORD cChars = NULL;   // DWORD: 32 bit u_int
	HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);

	if (std == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Could not retrieve standard handle\n (%d)", GetLastError()); // L for indicating wide chars
	}

	if (argv[1])
	{
		/* first arg of writeconsolew must be a handle to the console screen buffer with GENERIC_WRITE access right
		   GetStdHandle returns a handle with GENERIC_READ and GENERIC_RIGHT access rights

		   second arg is expected to be an array of wchar_t, as we're using writeconsolew

		   wcslen: strlen version for wide chars

		   fourth arg returns how many chars were actually written

		   last arg must be null, it is reserved
		*/

		WriteConsoleW(std, argv[1], wcslen(argv[1]), cChars, NULL);
	}

	CloseHandle(std);

	/*
	   why do i need to close handles?
	   from stackoverflow:


		Handles are a limited resource that occupy both kernel and userspace memory. Keeping a handle alive not only takes 
		an integer worth of storage, but also means that the kernel has to keep the thread information (such as user time, 
		kernel time, thread ID, exit code) around, and it cannot recycle the thread ID since you might query it using that handle.
		Therefore, it is best practice to close handles when they are no longer needed.

		This is what you are to do per the API contract (but of course you can break that contract).

			What will happens if I will not do so?

		Well, to be honest... nothing. You will leak that handle, but for one handle the impact will not be measurable. When your
		process exits, Windows will close the handle.

		Still, in the normal case, you should close handles that are not needed any more just like you would free memory that
		you have allocated (even though the operating system will release it as well when your process exits).

		Although it may even be considered an "optimization" not to explicitly free resources, in order to have a correct program,
		this should always be done. Correctness first, optimization second.
		Also, the sooner you release a resource (no matter how small it is) the sooner it is available again to the system for reuse.
	
		Other than that, you can also lose data and get corrupt, half-written files if you do not close (or at least sync) C 
		stdio handles and your process terminates unexpectedly. The reason is that your writes are buffered in user space,
		which goes kaboom once the process dies (similar could in theory happen with overlapped writes, 
		but can't happen in practice due to design). 

		None of that can possibly happen with a thread handle, however. Also, none of that should be possible with a
		Windows file handle (writes either succeed or fail, and once they succeeded, the responsibility is with the OS).

		If you don't close the handle, then it will remain open until your process terminates. Depending on what's behind the handle,
		this can be bad. Resources are often associated to handles and these won't be cleaned up until your program terminates;
		if you only use a few handles and these happen to be 'lightweight' then it doesn't really matter. Other handles, 
		such as file handles, have other side-effects to keeping the handle opened, for instance locking an opened file until your
		process exits. This can be very annoying to the user or other applications.

		In general, it's best to clean-up all handles, but at the end of the process, all handles are closed by Windows.

		... in conclusion: nothing may happen, but its best practice 
	*/

	return 0;
}