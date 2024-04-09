#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int
file_open(char* path) {
    struct stat st;
    int fd;
    if ((fd = open(path, 0) < 0)) {
        fprintf(2, "sln: file on path %s does not exist \n", path);
		return -1;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "sln: cannot stat %s\n", path);
		close(fd);
		return -1;
    }

    close(fd);
    return 0;
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
        if (file_open(target) < 0) {
            exit();
        }

        char* linkname = argv[2];

        symlink(target, linkname);
    }
	exit();
}
