struct memchunk{
	void *start;
	unsigned long length;
	int RW;

};
int gem_mem_layout(struct memchunk *chunk_list, int size);
