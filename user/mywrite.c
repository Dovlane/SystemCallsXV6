#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[])
{
    if (argc == 3) {
        int fd;
        struct stat st;
        char* path = argv[1];
        char* content = argv[2];

        if ((fd = open(path, 0) < 0)) {
            fprintf(2, "mywrite: cannot open %s\n", path);
            return -1;
        }

        if(fstat(fd, &st) < 0){
            fprintf(2, "mywrite: cannot stat %s\n", path);
            close(fd);
            return -1;
        }
        
        mywrite(fd, content, strlen(content) + 1);
        close(fd);
        // fprintf(2, "mywrite path: %s\n", path);
        // fprintf(2, "mywrite content: %s\n", content);

        
    }
    else {
        printf("write needs only 2 arguments. First is path, and second is content to write on the givent path. \n");
    }
    exit();
}
