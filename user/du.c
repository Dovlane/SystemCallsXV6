#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

#define O_NOFOLLOW 0x400

static int
strncmp(const char *p, const char *q, uint n)
{
	while(n > 0 && *p && *p == *q)
		n--, p++, q++;
	if(n == 0)
		return 0;
	return (uchar)*p - (uchar)*q;
}

char*
fmtname(char *path)
{
	static char buf[DIRSIZ+1];
	char *p;

	// Find first character after last slash.
	for(p=path+strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	// Return blank-padded name.
	if(strlen(p) >= DIRSIZ) {
		return p;
	}
	memmove(buf, p, strlen(p));
	memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	return buf;
}

int
du(char *path)
{
    char buf[512], *p;
	int fd, blocks_sum;
	struct dirent de;
	struct stat st;

	if((fd = open(path, O_NOFOLLOW)) < 0){
		fprintf(2, "du: cannot open %s\n", path);
		return 0;
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "du: cannot stat %s\n", path);
		close(fd);
		return 0;
	}

    blocks_sum = st.blocks;
    switch(st.type){
	case T_FILE:
    case T_SYMLINK:
    case T_DEV:
		printf("%s %d\n", fmtname(path), st.blocks);
		break;

	case T_DIR:
		if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
			printf("du: path too long\n");
			break;
		}
        printf("%s %d\n", fmtname(path), st.blocks);
		strcpy(buf, path);
		p = buf+strlen(buf);
		*p++ = '/';
        // p -> name of current inode
        // buf -> relative path to current inode
		while(read(fd, &de, sizeof(de)) == sizeof(de)){
			if(de.inum == 0)
				continue;
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			if(stat(buf, &st) < 0){
				printf("du: cannot stat %s\n", buf);
				continue;
			}

			switch(st.type){
			case T_DIR:
                if(strncmp(p, ".", DIRSIZ) != 0 && strncmp(p, "..", DIRSIZ) != 0)
                    blocks_sum += du(buf);
                break;

			case T_FILE:
            case T_DEV:
            case T_SYMLINK:
                blocks_sum += st.blocks;
				printf("%s %d\n", fmtname(buf), st.blocks);
				break;
            }
		}
		break;
	}
	close(fd);

    return blocks_sum;
}

int
main(int argc, char *argv[])
{
	int i;
	int blocks_sum = 0;
	if(argc < 2){
		blocks_sum = du(".");
	}
    char total[6] = {'t', 'o', 't', 'a', 'l', '\0'};
    
	for(i=1; i<argc; i++)
		blocks_sum += du(argv[i]);
    
    printf("%s %d\n", fmtname(total), blocks_sum);
	exit();
}
