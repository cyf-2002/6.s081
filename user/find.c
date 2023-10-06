#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", path);
        // 关闭文件描述符 fd
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        //如果类型为文件且名称与要查找的文件名相同则输出路径，如果是目录类型则递归调用 find() 函数继续查找。
        if (st.type == T_DIR)
        {
            find(buf, file);
        }
        else if (st.type == T_FILE && !strcmp(de.name, file))
        {
            printf("%s\n", buf);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "Please provide at least 3 parameters.\n");
        exit(1);
    }
    for (int i = 2; i < argc; i++)
    {
        find(argv[1], argv[i]);
    }
    exit(0);
}