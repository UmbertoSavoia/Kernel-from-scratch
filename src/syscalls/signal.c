#include "../../include/kernel.h"
#include "../../include/signal.h"
#include "../../include/process.h"

void exec_process_signal(t_process *process)
{
    int i = 0;

    if (!(process->signals[0]))
        return;
    process->task->registers.eip = (uint32)process->signals[0]->handler;

    kfree(process->signals[0]);
    for (i = 1; i < MAX_QUEUE_SIGNALS; ++i)
        process->signals[i - 1] = process->signals[i];
    process->signals[i - 1] = 0;
}

int push_back_queue_signal(t_process *process, int signum, sighandler_t handler)
{
    int i = 0;
    for (i = 0; i < MAX_QUEUE_SIGNALS; ++i)
        if (!(process->signals[i]))
            break;
    process->signals[i] = kmalloc(sizeof(t_signal));
    process->signals[i]->signum = signum;
    process->signals[i]->handler = handler;
    return (i == 0) ? 0 : process->signals[i - 1]->signum;
}