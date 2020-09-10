#include <stdlib.h> 

int b_size, n_blocks;
//-------Initialize empty disk - basiacally create empty vdisk file. Easy!
int disk_init_formatted(char *filename, int size, int num_blocks);

//-------Initialize (open) existing disk
int disk_init(char *filename, int size, int num_blocks);

void read_blocks(int start_ptr, void* data);

void write_blocks(int start_ptr, void* data);

static void check_disk(int block_num);

void terminate_disk();
