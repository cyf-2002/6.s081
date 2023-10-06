#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int pipe_read)
{
    int first;
    if (read(pipe_read, &first, sizeof(int)) != 0)
    {
        printf("prime %d\n", first);
    }
    else
    {
        close(pipe_read);
        exit(0);
    }

    int p[2];
    pipe(p);
    int num;
    if (fork() == 0)
    {
        while (read(pipe_read, &num, sizeof(int)) != 0)
        {
            if (num % first != 0)
            {
                write(p[1], &num, sizeof(int));
            }
        }
        close(pipe_read);
        close(p[1]);
        primes(p[0]);
    }
    else
    {
        close(p[0]);
        close(p[1]);
        wait(0);
    }

    exit(0);
}

int main()
{
    int p[2];
    pipe(p);
    int nums[34];

    for (int i = 0; i < 34; i++)
    {
        nums[i] = i + 2;
        write(p[1], &nums[i], sizeof(int));
    }
    close(p[1]);
    primes(p[0]);
    close(p[0]);
    exit(0);
}