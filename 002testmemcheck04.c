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

//参考 cat /usr/include/malloc.h   的__malloc_hook实现
typedef void*(*malloc_hoot_t)(size_t size,const void *caller);
malloc_hoot_t malloc_f;

typedef void*(*free_hook_t)(void *p, const void *caller);
free_hook_t free_f;

int replaced = 0; //标记系统的钩子函数是否被替换

//主要就这四个函数
void mem_trace(void);
void mem_untrace(void);
void *malloc_hook_f(size_t size, const void *caller);
void *free_hook_f(void *p, const void *caller);

void mem_trace(void)
{
	replaced = 1;
	malloc_f = __malloc_hook; //保存系统钩子函数的地址
	free_f = __free_hook;

	__malloc_hook = malloc_hook_f; //修改为自己的
	__free_hook = free_hook_f;
}
void mem_untrace(void)
{
	__malloc_hook = malloc_f; //再将系统的改回去
	__free_hook = free_f;
	replaced = 0;
}
void *malloc_hook_f(size_t size, const void *caller)
{
	mem_untrace();//先取消hook,用系统的malloc申请

	void *ptr = malloc(size);
	printf("caller:+%p: addr[%p]\n", caller, ptr);

	mem_trace();//申请完内存后还将系统的mallochook指向自己的

	return ptr;
}
void *free_hook_f(void *p, const void *caller)
{
	mem_untrace();

	printf("caller:-%p: addr[%p]\n", caller, p);
	free(p);

	mem_trace();
}

int main(int argc, char **argv)
{
	mem_trace(); //开启内存使用记录

	void *p1 = malloc(10);
	void *p2 = malloc(20);

	free(p1);

	mem_untrace(); //取消内存使用记录

	return 0;
}



