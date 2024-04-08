#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int
file_exist(char* path, int* exist) {
    struct stat st;
    int fd;
    *exist = 0;
    if ((fd = open(path, 0) < 0)) {
        fprintf(2, "sln: cannot open %s\n", path);
        exit();
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "sln: cannot stat %s\n", path);
        close(fd);
        exit();
    }
    close(fd);
    *exist = 1;
}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("too few arguments for symlink\n");
    }
    else if (argc == 3) {
        int fd;
        struct stat st;
        char* target = argv[1];

        int* exist;
        file_exist(target, exist);
        if (!(*exist)) {
            exit();
        }

        char* linkname = argv[2];

        symlink(target, linkname);
    }
	exit();
}
