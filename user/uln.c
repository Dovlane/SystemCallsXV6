#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	if(argc != 2){
		fprintf(2, "Usage: uln path\n");
		exit();
	}
	if(unlink(argv[1]) < 0)
		fprintf(2, "ulink %s: failed\n", argv[1]);
	exit();
}