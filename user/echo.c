#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	int i;
// echo 123 > a
	for(i = 1; i < argc; i++)
		printf("%s%s", argv[i], i+1 < argc ? " " : "\n");
	exit();
}
