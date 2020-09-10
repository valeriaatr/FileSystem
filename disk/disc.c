//disk emulator
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "disk.h"

// Create global variables for disk
FILE* fp = NULL;
int b_size, n_blocks;

//-------Initialize empty disk - basiacally create empty vdisk file. Easy!
int disk_init_new(char *filename, int size, int num_blocks){
	b_size = size;//512
	n_blocks = num_blocks;//4096
	
	fp = fopen(filename, "w+"); //w+ because we create at init
	if (fp==NULL){
		printf("Error creating a disk: %s\n", filename);
		return 1;
	}	
	//fill the file with specified size
	ftruncate(fileno(fp),b_size*n_blocks);
	printf("Disk successfuly allocated");
    return 0;
}
//-------Initialize (open) existing disk
// Here, we are able to append data
int disk_init(char *filename, int size, int num_blocks){
	b_size = size;
	n_blocks = num_blocks;
	
	fp = fopen(filename, "r+"); //r+ because we read and write
	if (fp==NULL){
		printf("Error opening a disk: %s\n", filename);
		return 1;
	}
	printf("Disk successfuly opened");
	return 0;
}

//Now, recall fseek() is used to move file pointer

//------- Read blocks from disk
void read_blocks(int start_ptr, void* data){
	check_disk(start_ptr);
	fseek(fp, start_ptr*b_size, SEEK_SET); //seek by address specified 
	fread(data,b_size,1, fp); //fread() reads block of data from STREAM; 1 is num of elements
}
	
//------- Write blocks to disk
void write_blocks(int start_ptr, void* data){
	check_disk(start_ptr);
	fseek(fp, start_ptr*b_size,SEEK_SET);
	fwrite(data, b_size, 1, fp);
}

//------- Invalid Input safeguard
static void check_disk(int block_num){
	if(block_num<0) {
		printf("ERROR: blocknum (%d) negative\n",block_num);
		exit(1);
	}
	if(block_num>=n_blocks) {
		printf("ERROR: blocknum (%d) is too big\n",block_num);
		exit(1);
	}
}

void terminate_disk(){
	if (fp!=NULL)
		fclose(fp);
	printf("Disk closed");
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
