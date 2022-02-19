#ifndef SIGNAL_H
#define SIGNAL_H

#include "process.h"

#define SIGHUP        1          // Hangup detected on controlling terminal or death of controlling process
#define SIGINT        2          // Interrupt from keyboard
#define SIGQUIT       3          // Quit from keyboard
#define SIGILL        4          // Illegal Instruction
#define SIGTRAP       5          // Trace/breakpoint trap
#define SIGABRT       6          // Abort signal from abort(3)
#define SIGBUS        7          // Bus error (bad memory access)
#define SIGFPE        8          // Floating-point exception
#define SIGKILL       9          // Kill signal
#define SIGUSR1      10          // User-defined signal 1
#define SIGSEGV      11          // Invalid memory reference
#define SIGUSR2      12          // User-defined signal 2
#define SIGPIPE      13          // Broken pipe: write to pipe with no readers
#define SIGALRM      14          // Timer signal from alarm(2)
#define SIGTERM      15          // Termination signal
#define SIGSTKFLT    16          // Stack fault on coprocessor (unused)
#define SIGCHLD      17          // Child stopped or terminated
#define SIGCONT      18          // Continue if stopped
#define SIGSTOP      19          // Stop process
#define SIGTSTP      20          // Stop typed at terminal
#define SIGTTIN      21          // Terminal input for background process
#define SIGTTOU      22          // Terminal output for background process
#define SIGURG       23          // Urgent condition on socket (4.2BSD)
#define SIGXCPU      24          // CPU time limit exceeded (4.2BSD)
#define SIGXFSZ      25          // File size limit exceeded (4.2BSD)
#define SIGVTALRM    26          // Virtual alarm clock (4.2BSD)
#define SIGPROF      27          // Profiling timer expired
#define SIGWINCH     28          // Window resize signal (4.3BSD, Sun)
#define SIGIO        29          // I/O now possible (4.2BSD)
#define SIGPWR       30          // Power failure (System V)
#define SIGSYS       31          // Bad system call (SVr4)

#define SIG_ERR (void (*)())-1
#define SIG_DFL (void (*)())0
#define SIG_IGN (void (*)())1

typedef void (*sighandler_t)(int);
int push_back_queue_signal(t_process *process, int signum, sighandler_t handler);
void exec_process_signal(t_process *process);

#endif