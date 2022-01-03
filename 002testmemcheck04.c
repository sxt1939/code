/*
 * 002testmemcheck01.c
 *
 *  Created on: 2022��1��3��
 *      Author: LENOVO
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

//�ο� cat /usr/include/malloc.h   ��__malloc_hookʵ��
typedef void*(*malloc_hoot_t)(size_t size,const void *caller);
malloc_hoot_t malloc_f;

typedef void*(*free_hook_t)(void *p, const void *caller);
free_hook_t free_f;

int replaced = 0; //���ϵͳ�Ĺ��Ӻ����Ƿ��滻

//��Ҫ�����ĸ�����
void mem_trace(void);
void mem_untrace(void);
void *malloc_hook_f(size_t size, const void *caller);
void *free_hook_f(void *p, const void *caller);

void mem_trace(void)
{
	replaced = 1;
	malloc_f = __malloc_hook; //����ϵͳ���Ӻ����ĵ�ַ
	free_f = __free_hook;

	__malloc_hook = malloc_hook_f; //�޸�Ϊ�Լ���
	__free_hook = free_hook_f;
}
void mem_untrace(void)
{
	__malloc_hook = malloc_f; //�ٽ�ϵͳ�ĸĻ�ȥ
	__free_hook = free_f;
	replaced = 0;
}
void *malloc_hook_f(size_t size, const void *caller)
{
	mem_untrace();//��ȡ��hook,��ϵͳ��malloc����

	void *ptr = malloc(size);
	printf("caller:+%p: addr[%p]\n", caller, ptr);

	mem_trace();//�������ڴ�󻹽�ϵͳ��mallochookָ���Լ���

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
	mem_trace(); //�����ڴ�ʹ�ü�¼

	void *p1 = malloc(10);
	void *p2 = malloc(20);

	free(p1);

	mem_untrace(); //ȡ���ڴ�ʹ�ü�¼

	return 0;
}



