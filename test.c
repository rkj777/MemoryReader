#include "memchunk.h"
#include <stdio.h>

int main(){
	int size = 30;
	int total_size;
	int i;
	struct memchunk memchunk_list[30];
	total_size = gem_mem_layout(memchunk_list,size);
	printf("The Total Number of Chunks: %d\n" , total_size);
	for(i = 0; i < total_size; i++){
		printf("Chunk %d: Read status = %d\n" , (i+1), memchunk_list[i].RW );
	}
}

