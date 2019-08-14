#include "backtrace.h"

#include "SDL.h"

#define USE_EXECINFO defined(__GNUC__) && !defined(__MINGW32__) && !defined(__OpenBSD__) && !defined(__vita__) && !defined(__SWITCH__)

// Only use Windows' StackWalk functions if we're compiling on MSVC or MINGW-W64 (MINGW classic does not include dbghelp.h)
#define USE_STACKWALK defined(_WIN32) && (defined(__MINGW64_VERSION_MAJOR) || defined(_MSVC))

#ifdef __MINGW32__
#include <_mingw.h>
#endif

#if USE_EXECINFO
#include <execinfo.h>
#endif

#ifndef _WIN32
#include <signal.h>
#include <stdio.h>
#endif

#ifdef USE_STACKWALK

#include <windows.h>
#include <dbghelp.h>

#include <stdio.h>
#include <string.h>

#ifdef _WIN64
static void windows_print_stacktrace(CONTEXT* context)
{
    HANDLE process = GetCurrentProcess();

    if (!SymInitialize(process, 0, TRUE)) {
        return;
    }

    STACKFRAME64 frame = { 0 };
    frame.AddrPC.Offset = context->Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Rsp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Rbp;
    frame.AddrFrame.Mode = AddrModeFlat;

    while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, process, GetCurrentThread(), &frame, context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
        // extract data
        DWORD64 functionAddress = frame.AddrPC.Offset;
        const char *functionName = 0;
        const char *fileName = 0;
        unsigned int lineNumber = 0;

        char symbolBuffer[sizeof(IMAGEHLP_SYMBOL64) + 255];
        PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)symbolBuffer;
        symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64) + 255;
        symbol->MaxNameLength = 254;

        if (SymGetSymFromAddr64(process, frame.AddrPC.Offset, NULL, symbol)) {
            functionName = symbol->Name;
        }

        DWORD offset = 0;
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &offset, &line)) {
            fileName = line.FileName;
            lineNumber = line.LineNumber;
        }
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "  0x%08I64x %s %s (%d)", functionAddress, functionName, fileName, lineNumber);
    }

    SymCleanup(process);
}
#else
static void windows_print_stacktrace(CONTEXT* context)
{
    HANDLE process = GetCurrentProcess();

    if (!SymInitialize(process, 0, TRUE)) {
        return;
    }

    STACKFRAME frame = { 0 };
    frame.AddrPC.Offset = context->Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;

    while (StackWalk(IMAGE_FILE_MACHINE_I386, process, GetCurrentThread(), &frame, context, 0, SymFunctionTableAccess, SymGetModuleBase, 0)) {
        // extract data
        DWORD64 functionAddress = frame.AddrPC.Offset;
        const char *functionName = 0;
        const char *fileName = 0;
        unsigned int lineNumber = 0;

        char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
        PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
        symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) + 255;
        symbol->MaxNameLength = 254;

        if (SymGetSymFromAddr(process, frame.AddrPC.Offset, NULL, symbol)) {
            functionName = symbol->Name;
        }

        DWORD offset = 0;
        IMAGEHLP_LINE line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

        if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset, &line)) {
            fileName = line.FileName;
            lineNumber = line.LineNumber;
        }
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "  0x%08I64x %s %s (%d)", functionAddress, functionName, fileName, lineNumber);
    }

    SymCleanup(process);
}
#endif

static LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS *exceptionInfo)
{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Oops, crashed with code %ld, generating stacktrace...", exceptionInfo->ExceptionRecord->ExceptionCode);
    windows_print_stacktrace(exceptionInfo->ContextRecord);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Stacktrace complete");

    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "Julius crashed unexpectedly",
        "Please open an issue at:\nhttps://github.com/bvschaik/julius\n\nand include julius-log.txt",
        NULL
    );

    return EXCEPTION_EXECUTE_HANDLER;
}

void platform_backtrace_install(void)
{
    SetUnhandledExceptionFilter(windows_exception_handler);
}

#else // non-windows or windows without stackwalk

static void handler(int sig) {
#if USE_EXECINFO
    void *array[100];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 100);

    // print out all the frames to log
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: signal %d:", sig);
    char **lines = backtrace_symbols(array, size);
    if (lines) {
        for (int i = 0; i < size; i++) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", lines[i]);
        }
        free(lines);
    }
#else
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Oops, crashed with signal %d :(", sig);
#endif
    exit(1);
}

void platform_backtrace_install(void)
{
    signal(SIGSEGV, handler);
}

#endif
