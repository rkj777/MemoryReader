#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include "memchunk.h"

//Global variables to be used due to sigsetjump and siglongjump
static jmp_buf jump_pointer;
char *p=0;
uint32_t pagesize;
uint32_t page_count = 0;
int RW;
//An Rw_prev that is impossible to get to for the first loop
int RW_prev = 2;
 

//Signal catcher function
void catch_nonreadable(int signum){
	siglongjmp(jump_pointer, signum);
}


int gem_mem_layout(struct memchunk *chunk_list, int size){
	//Getting the number of pages to loop 
	pagesize = getpagesize(); 
	uint32_t number_of_pages =((1<<31)/pagesize)*2;
	
	//Setting up the variables to read data and length of chunks
	char readable;
	int memchunk_list_counter = 0;
	int total_chunk_length = 0;
	unsigned long current_chunk_length = 0;

	//First iteration of the loop to set Rw_prev
	signal(SIGSEGV,catch_nonreadable);
	if (sigsetjmp(jump_pointer, 1) == 0){
		RW = -1;
			
		readable = *p;
		RW = 0;
			
		*p = readable;
		RW  = 1;	
	}
	RW_prev = RW;
	p+=pagesize;
	page_count++;
	current_chunk_length +=pagesize;
	
	//The loop to go through the entire memory space
	while(page_count <= number_of_pages){
		//Condtion is only satisifed if siglongjump has not been called in this iteration 
		if (sigsetjmp(jump_pointer, 1) == 0){
			RW = -1;
			
			readable = *p;
			RW = 0;
			
			*p = readable;
			RW  = 1;	
		}
		//Checks for a new chunk
		if(RW != RW_prev){
			total_chunk_length++;
			//If the size of the memchunk list given is less then the current chunks found
			if(memchunk_list_counter <= size){
				chunk_list[memchunk_list_counter].start = (void*) ((unsigned long)p - current_chunk_length - pagesize);
				chunk_list[memchunk_list_counter].length = current_chunk_length;
				chunk_list[memchunk_list_counter].RW = RW_prev;
				
				memchunk_list_counter++;
				current_chunk_length = pagesize;
			}
			//Starting a new chunk
			RW_prev = RW;
			p+=pagesize;
			page_count++;
		}
		//continuing the old chunk
		else{
			p+=pagesize;
			page_count++;
			current_chunk_length+=pagesize;
		}
	}
	return total_chunk_length;	
 }		


