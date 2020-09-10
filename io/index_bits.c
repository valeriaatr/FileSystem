#include <stdio.h>

unsigned char buffer[512];

void init_buffer(unsigned char buffer[], int size)
{
    for (int i = 0; i < size; i++) {
        buffer[i] = 0x0;
    }
}

void print_buffer(unsigned char buffer[], int size)
{
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

void set_block(unsigned char buffer[], int block_num)
{
    int index = block_num / 8;
    int bit_index = block_num % 8;
    
    //printf("SET\n");
    //printf("Index: %d\n", index);
    //printf("Bit Index: %d\n", bit_index);
    buffer[index] |= 1UL << bit_index;
}

void unset_block(unsigned char buffer[], int block_num)
{
    int index = block_num / 8;
    int bit_index = block_num % 8;
    
    //printf("UNSET\n");
    //printf("Index: %d\n", index);
    //printf("Bit Index: %d\n", bit_index);
    buffer[index] &= ~(1UL << bit_index);
}

int main()
{
    init_buffer(buffer, 512);
    print_buffer(buffer, 512);
    
    // Each byte represents 8 blocks
    set_block(buffer, 5);
    
    print_buffer(buffer, 512);
    
    unset_block(buffer, 5);
    
    print_buffer(buffer, 512);
    
    set_block(buffer, 579);
    
    print_buffer(buffer, 512);
    
    // Remember we're indexing from 0 so block 8 will be in the 1st 
    // bit of the second byte
    set_block(buffer, 8);
    
    print_buffer(buffer, 512);
    
}