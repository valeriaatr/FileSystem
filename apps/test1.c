#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "File.h"

int main(int argc,char* argv[]){
	char filename[]="test1.txt";
	FILE *file = fopen(filename,"w");
	char str[]="Test 1 - Write to Disk";
	fwrite(str,1,sizeof(str),file);
	
	int new =1; //shows that file is new
	
	make_fs(new);
	write_file(file,filename,sizeof(filename));
	printf("File written successfully");
}
