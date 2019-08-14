#ifndef PLATFORM_BACKTRACE_H
#define PLATFORM_BACKTRACE_H

/**
 * Install signal/exception handler for crash logging
 */
void platform_backtrace_install(void);

#endif // PLATFORM_BACKTRACE_H
