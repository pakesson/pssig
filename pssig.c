#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/**
    The maximum process ID can be found in /proc/sys/kernel/pid_max,
    but since we're assuming execution without access to /proc the cap
    is instead set to the current PID by default.

    This works out okay in most cases, since process IDs are likely(tm) to be
    sequential and increasing. This is not guaranteed though. See
    for example the POSIX.1-2008 standard
        http://pubs.opengroup.org/onlinepubs/9699919799/
    for the requirements put on process IDs.
 */

#define INIT_PID 1

int main(int argc, char *argv[])
{
    pid_t pid, current_pid;
    int errsv;

    current_pid = getppid(); // Parent process ID

    printf("Current PID: %d\n", current_pid);
    printf("Searching for processes...\n");

    for (pid = INIT_PID; pid < current_pid; pid++) 
    {
        if (!kill(pid, 0))
        {
            printf("[%d] Found PID\n", pid);
        }
        else
        {
            errsv = errno;
            switch (errsv)
            {
                case EPERM:
                    printf("[%d] Access denied\n", pid);
                    break;
                case EINVAL: // Invalid signal
                case ESRCH: // PID does not exist
                    break;
                default:
                    printf("[%d] Unknown error %d\n", pid, errsv);
                    break;
            }
        }
    }

    return 0;
}
