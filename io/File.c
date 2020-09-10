#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include "../disk/disk.h"

#include "File.h" // NOT HERE YET

#define BLOCK_SIZE 512
#define NUM_BLOCKS 4096
#define MAGIC_NUMBER 0xf0f03410
#define NUM_OF_INODES_BLOCKS 6 

//-------     Bit Operations for Free Block Vector   --------------

void init_buffer(unsigned char buffer[], int size){
    for (int i = 0; i < size; i++) {
        buffer[i] = 0x0;
    }
}
void print_buffer(unsigned char buffer[], int size){
    printf("%04x: ", 0);
    for (int i = 1; i <= size; i++) {
        printf("%02x ", buffer[i-1]);
        if(i % 8 == 0)
        {
            printf("\n");
            printf("%04x: ", i);
        }
    }
    printf("\n");
}
void set_block(unsigned char buffer[], int block_num){
    int index = block_num / 8;
    int bit_index = block_num % 8;
    buffer[index] |= 1UL << bit_index;
}

void unset_block(unsigned char buffer[], int block_num){
    int index = block_num / 8;
    int bit_index = block_num % 8;

    buffer[index] &= ~(1UL << bit_index);
}

//-------Each iNode is 32b, with max size of iNode Block 512b. Then, each iNode Block has 16 inodes.
struct iNode{
	int filetype; //0 for dir, 1 for file
	int size;
	int block_ptr[6]; //max allocation per file
	//int indir_block_ptr=NULL; //array of pointers??
};

struct SuperBlock{
	unsigned char magic;
	int blocks;
	int blocks_for_inodes; //409 is 10% from total num of blocks
	int num_inodes; //16 inodes per block of 512b
};
struct iTable{
	iNode node;
	char filename[15];
};

struct root{
	char rt[2];
	iTable dir_tree[NUM_OF_INODES_BLOCKS]; //this is just a mapping that will show hierarchy as dirs are created
};

unsigned char b_vector[512];//used to track used and unused data blocks
unsigned char buffer[512];

int start_ptr=5;
//------Make FS, i.e. specify disk and static nodes
//if Disk doesn't exist, allocate it. If it does, read root and inodes blocks
void make_fs(int new){
	
	if(new){
		
		struct SuperBlock sb;
		sb.magic = MAGIC_NUMBER;
		sb.blocks_for_inodes = NUM_OF_INODES_BLOCKS;
		sb.num_inodes = blocks_for_inodes*16;
		sb.blocks = NUM_BLOCKS;
		
		struct root r;
		r.rt="/";
	
		for(int i=0;i<NUM_OF_INODES_BLOCKS;i++){
			struct iTable dir_tree;
			struct iNode node;
			r.dir_tree[i]->filename="";
			r.dir_tree[i]->node.filetype=1;
			r.dir_tree[i]->node.size=0;
			for(int j=0;i<NUM_OF_INODES_BLOCKS;j++){
				r.dir_tree[i]->node.block_ptr[j]=0;
			}
		}
		
		init_buffer(b_vector,BLOCK_SIZE);
		
		//first 10 (0-9) blocks are taken
		for(int i=0;i<10;i++){
			set_block(buffer, i);
		}
		
		disk_init_new("../disk/vdisk", BLOCK_SIZE*NUM_BLOCKS, NUM_BLOCKS);
		write_blocks(0,sb);// write superblock first
		//now write all dummy inodes. They are updated as files being created
		write_blocks(1,b_vector);
		//for(int i=2; i<8;i++){ //we have 6 inodes. Small number is just for learning FS
		//	write_blocks(i,inode);
		//}
		//write_blocks(8,root);
	}
	else {
		disk_init("../disk/vdisk",BLOCK_SIZE*NUM_BLOCKS, NUM_BLOCKS);
	}
	printf("File System initiated");
}

//display content of a FS
//void list_fs(){
//	int i;
	
	
//}


int write_file(FILE* fp, char fname, int size){
	//make sure block gets allocated 
	int i=0;
	int j=0;//this is block pointer index for inode. 5 max.
	int num_taken_blocks=0;
	int c;
	
	if (size<512){ //if it takes 1 block
		while(i<sizeof(buffer)){ //fill 512B
			c=fgetc(fp);
			if (feof(fp)){
				break;
			}
			buffer[i]=c;
			i++;	
		}
		set_block(start_ptr,b_vector);//set allocated block
		write_blocks(start_ptr,buffer);
		memset(buffer,0,sizeof(buffer));
		printf("Blocks taken: %d\n",num_taken_blocks);
		num_taken_blocks++;

		root.dir_tree[start_ptr]->filename=fname;
		root.dir_tree[start_ptr]->node.filetype=1;
		root.dir_tree[start_ptr]->node.size=size;
		root.dir_tree[start_ptr]->node.block_ptr[j]=start_ptr;
		start_ptr++;
		
		return num_taken_blocks;
	}
	else{ //if size takes several blocks
	//generally, it should not be sequential allocation, and it\
	must search next AVAILABLE free block, instead of taking just next one
		while(i<size){ //fill >512B
			c=fgetc(fp);
			buffer[i]=c;
			i++;
			if (i==512){//once you filled buffer
				set_block(start_ptr,b_vector);//set allocated block
				write_blocks(start_ptr,buffer);
				memset(buffer,0,sizeof(buffer));
				num_taken_blocks++;
				root.dir_tree[start_ptr]->filename=fname;
				root.dir_tree[start_ptr]->node.filetype=1;
				root.dir_tree[start_ptr]->node.size=size;
				root.dir_tree[start_ptr]->node.block_ptr[j]=start_ptr;
				start_ptr++;//point to next block
				i=0;
				j++;
			}
			if (c==feof){//finished reading file
				set_block(start_ptr,b_vector);//set allocated block
				write_blocks(start_ptr,buffer);
				memset(buffer,0,sizeof(buffer));
				num_taken_blocks++;
				root.dir_tree[start_ptr]->filename=fname;
				root.dir_tree[start_ptr]->node.filetype=1;
				root.dir_tree[start_ptr]->node.size=size;
				root.dir_tree[start_ptr]->node.block_ptr[j]=start_ptr;
				start_ptr++;	
				break;
			}
		}
		printf("Blocks taken: %d\n",num_taken_blocks);
		return num_taken_blocks;
	}
}

//int read_file(int id, char *fname){
	
//}

//int del_file(int id, char *fname){
	
//}

//int log(){ //log is used to watch and keep a file if system fails (?)
	
//}


















