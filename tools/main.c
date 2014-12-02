#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "structure.h"

extern SimpleFileSystem sfscreate (char *sfsName);
extern void sfsadd(SimpleFileSystem *sfs , char filename[32]);
extern void sfsls(SimpleFileSystem sfs);
extern void sfsdel(SimpleFileSystem *sfs, char filename[32]);

int main(int argc, char **argv){
	SimpleFileSystem sf = sfscreate("sfs");
	sfsadd(&sf, "test.txt\0");
	sfsadd(&sf, "fichier.txt\0");
	sfsls(sf);
	sfsdel(&sf, "test.txt");
	sfsls(sf);
}