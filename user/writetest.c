#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int
file_open(char* path) {
    struct stat st;
    int fd;
    if ((fd = open(path, 0) < 0)) {
        fprintf(2, "write: cannot open %s\n", path);
		return -1;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "write: cannot stat %s\n", path);
		close(fd);
		return -1;
    }
    close(fd);
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc == 2) {
        int fd;
        struct stat st;
        char* path = argv[1];

        int* exist;
        if (file_open(path) < 0) {
            exit();
        }

        writetest(path);
    }
    else {
        printf("write needs only 1 argument\n");
    }
    exit();
}
