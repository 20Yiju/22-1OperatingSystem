//22000690
/*
	This program translates logical address into physical address using the paging technique.
	Complement the program following the instruction.

	The program should work for other page table with different total_bits, offset_bits, and no_pages.

	Ex)
		$ cat pagetable1.txt		# page table file
		4 2							# total_bits = 4, offset_bits = 2
		4							# the number of pages
		5 6 1 2						# mapping from pages to frames

		$ ./a.out pagetable1.txt
		reading page table from file pagetable1.txt.
		total_bits = 4, offset_bits = 2, no_page = 4
		offset_mask = 0x3
		max_addr = 15
		0000) 0 (0x00000000) -> 20 (0x00000014)
		0001) 1 (0x00000001) -> 21 (0x00000015)
		0002) 2 (0x00000002) -> 22 (0x00000016)
		0003) 3 (0x00000003) -> 23 (0x00000017)
		0004) 4 (0x00000004) -> 24 (0x00000018)
		0005) 5 (0x00000005) -> 25 (0x00000019)
		0006) 6 (0x00000006) -> 26 (0x0000001a)
		0007) 7 (0x00000007) -> 27 (0x0000001b)
		0008) 8 (0x00000008) -> 4 (0x00000004)
		0009) 9 (0x00000009) -> 5 (0x00000005)
		0010) 10 (0x0000000a) -> 6 (0x00000006)
		0011) 11 (0x0000000b) -> 7 (0x00000007)
		0012) 12 (0x0000000c) -> 8 (0x00000008)
		0013) 13 (0x0000000d) -> 9 (0x00000009)
		0014) 14 (0x0000000e) -> 10 (0x0000000a)
		0015) 15 (0x0000000f) -> 11 (0x0000000b)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

typedef struct  {
	int no_page;				// the number of pages in the current process
	int total_bits;				// total number of bits in logical address
	int offset_bits;			// the number of bits for page offset (offset_bits < total_bits)
	int *frame_no;				// mapping from page number to frame number

	int offset_mask;			// bit mask for address translation
} PageTable;

void PT_Load(PageTable *pt, const char *filename);
void PT_Destroy(PageTable *pt);
int PT_Translate(PageTable *pt, int logical);

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s <pagetable_file>\n", argv[0]);
		return -1;
	}

	PageTable pt;
	PT_Load(&pt, argv[1]);

	int max_addr = pt.no_page * (1 << pt.offset_bits) - 1;

	printf("max_addr = %d\n", max_addr);

	int step = MIN(32, max_addr);

	for(int i = 0; i <= step; i++){
		int logical = (int)(max_addr * (i / (float)step));
		int physical = PT_Translate(&pt, logical);

		printf("%04d) %d (0x%08x) -> %d (0x%08x)\n", i, logical, logical, physical, physical);
	}

	PT_Destroy(&pt);

	return 0;
}

// functions for PageTable 


void PT_Load(PageTable *pt, const char *filename)
{
	printf("reading page table from file %s.\n", filename);

	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Failed to open file %s.\n", filename);
		exit(-1);
	}

	fscanf(fp, "%d", &pt->total_bits);
	fscanf(fp, "%d", &pt->offset_bits);
	fscanf(fp, "%d", &pt->no_page);

	printf("total_bits = %d, offset_bits = %d, no_page = %d\n",
		pt->total_bits, pt->offset_bits, pt->no_page);

	pt->frame_no = (int *)malloc(pt->no_page * sizeof(int));
	if(pt->frame_no == NULL){
		printf("Failed to allocate memory.\n");
		exit(-1);
	}
	
	for(int i = 0; i < pt->no_page; i++)
		fscanf(fp, "%d", &pt->frame_no[i]);

	fclose(fp);

	// TO DO: build offset_mask. Lower offset_bits should be 1 and the other bits should be zero
	// 	  	  e.g., if total_bits = 7 and offset_bits = 3, offset_mask should be 0000111
	// 	  	        if total_bits = 10 and offset_bits = 6, offset_mask should be 0000111111
	//		  the code should work for any combination of total_bits and offset_bits not exceeding 32
	
	pt->offset_mask = (1 <<  pt -> offset_bits ) - 1;

	printf("offset_mask = 0x%x\n", pt->offset_mask); 
}

void PT_Destroy(PageTable *pt)
{
	if(pt->frame_no){
		free(pt->frame_no);
		pt->frame_no = NULL;
	}

	pt->no_page = 0;
}

int PT_Translate(PageTable *pt, int logical)
// returns the physical address translated from the local address
{
	// TO DO: implement the translation algorithm using the page table
	//		Use bit-wise operators and offset_mask of pt
	//		Hints) 011010 & 000111 == 000010
	//		       011010 >> 3 	  == 000110
	//		       110000 | 000010 == 110010
	//		% and / operators are NOT ALLOWED because they are inefficient.
	//		if the page number is invalid (e.g. greater than or equal to PTLR),
	//			display an error message and terminate the process.
	//			e.g., "ERROR! Invalid page number 9."


	int page_num = logical >> pt -> offset_bits;
	int offset = logical & pt-> offset_mask;

	return pt->frame_no[page_num] << pt -> offset_bits | offset;


}
