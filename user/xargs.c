#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Please provide at least 2 parameters.\n");
        exit(1);
    }

    char *argvs[MAXARG];
    int index = 0;
    for (int i = 1; i < argc; i++)
    {
        argvs[index] = argv[i];
        index += 1;
    }

    // 缓冲区存放从管道读出的数据
    char buf[512] = {"\0"};
    char temp[512] = {"\0"};

    int n;
    if ((n = read(0, buf, 512)) == -1)
    {
        fprintf(2, "xargs: read error");
    }

    int j = 0;
    for (int i = 0; i < strlen(buf); ++i)
    {
        if (buf[i] == '\n')
        {
            argvs[index] = temp;
            //temp之前已经初始化了 但是不加这个会报错 没搞清楚是为什么
            temp[j] = '\0';
            if (fork() == 0)
            {
                exec(argv[1], argvs);
                exit(1);
            }
            else
            {
                wait(0);
                index = argc - 1;
                j = 0;
            }
        }
        else
        {
            temp[j++] = buf[i];
        }
    }

    exit(0);
}
