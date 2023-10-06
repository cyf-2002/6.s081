#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(2, "arg error\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    if(sleep(n) < 0){
        exit(1);
    }
    exit(0);
}
