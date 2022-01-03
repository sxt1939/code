/*
 * 002testmemcheck01.c
 *
 *  Created on: 2022年1月3日
 *      Author: LENOVO
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

extern void *__libc_malloc(size_t size);
int enable_malloc_hook = 1; //递归终止条件

extern void __libc_free(void* p);
int enable_free_hook = 1;

void *malloc(size_t size)
{
	if(enable_malloc_hook)
	{
		enable_malloc_hook = 0;//printf内部也会调用malloc.所以要加一个递归终止条件
		void *p = __libc_malloc(size);
		printf("malloc addr: %p,size:%ld\n",p,size);
		void *caller = __builtin_return_address(1); //1代表返回调用malloc的函数,2,3,类似于调用栈
//
		char buff[128] = {0};
		sprintf(buff,"./mem/%p.mem",p); //记得创建一个mem文件夹
		FILE *fp = fopen(buff,"w");
		fprintf(fp,"[+%p] --> malloc_addr:%p,size:%ld\n",caller,p,size);
		fflush(fp);
		//fclose(fp);

		enable_malloc_hook = 1;

		return p;
	}
	else
	{
		return __libc_malloc(size);
	}
}
void free(void *p)
{
	if(enable_free_hook)
	{
		printf("if free :%p\n",p);
		enable_free_hook = 0;
		char buff[128] = {0};
		sprintf(buff, "./mem/%p.mem", p);

		if (unlink(buff) < 0)
		{ // no exist
			printf("double free: %p\n", p);
		}
		__libc_free(p);
		// rm -rf p.mem
		enable_free_hook = 1;
	}
	else
	{
		__libc_free(p);
	}
}


int main(int argc, char **argv)
{
    void *p1 = malloc(10);
    void *p2 = malloc(10);
    free(p1);

	return 0;
}



