#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int ptoc[2], ctop[2];
    char buf[8];
    pipe(ptoc);
    pipe(ctop);

    if (fork() == 0)
    {
        read(ptoc[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        write(ctop[1], "pong", strlen("pong"));
    }
    else
    {
        write(ptoc[1], "ping", strlen("ping"));
        wait(0);
        read(ctop[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);
}