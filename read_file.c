#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fs.h"

int main(void)
{
	FILE* fptr;
	struct partition tf;//total file 
	int root_node; 
	struct inode* root_ptr;

	fptr = fopen("disk.img","r");
	if(fptr == NULL){
		perror("Error");
		return 1;
	}
	else{

		/* ======================READ SUPERBLOCK=========================*/
		fread(&tf.s,sizeof(struct super_block),1,fptr);
		printf("super block first_inode : %d\n",tf.s.first_inode);
		root_node = tf.s.first_inode;
		root_ptr = &tf.inode_table[root_node];

		/*======================Read inode array =========================*/
		for(int l = 0; l < 224; l ++)
			fread(&tf.inode_table[l],sizeof(struct inode),1,fptr);
		
		/*=====================Read data blocks=============================*/
		for(int l = 0; l < 4088; l ++)
			fread(&tf.data_blocks[l],sizeof(struct blocks),1,fptr);

               /*================Access to root node and find data blocks================*/    		
		printf("Access to first inode (Root node)\n");
		printf("Size of datablocks : %d \n",root_ptr -> size);
		int db_size =root_ptr -> size;
		int block_num = 0 ;
		int block_store[0x6];
		while(db_size > 0 ){

			block_store[block_num]=root_ptr->blocks[block_num];
			printf("data block : %d\n", block_store[block_num]);
			block_num ++;
			db_size = db_size - 1000;
		}

		/* ============================Read every file in root ================*/
		for(int l = 0; l< block_num; l ++){
			struct blocks* bp = &tf.data_blocks[block_store[l]]; 
			printf("Read block : %d\n",block_store[l]);
			struct dentry* den;
			char* sp = bp->d;
			int read_size = 0 ;
			while(*sp && read_size < 1024){
				den = (struct dentry *) sp;
				sp += den->dir_length;
				read_size += den->dir_length;
				printf("file name : %s\n",*den->n_pad);
			}	
		}



	}

	return 0;






}
