#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    pid_t pid = fork();
    /* the child process is a duplicate of the calling prosess;
    what IS a process? 
    what is memory locks*/
    if (pid)
    {
        // getpid returns current process PID, if it's a parent
        // pid = fork() is the childs PID 
        printf("This is parent process! My PID is: %d, Childs: %d\n", getpid(), pid);
    }
    else 
    {
        //getpid returns the current process PID, if its a child
        // getppid returns parents PID
        // we know this is the child because pid = 0, and not a childs PID
        printf("This is the child process! My PID: %d, Parents: %d\n", getpid(), getppid());
    }
}