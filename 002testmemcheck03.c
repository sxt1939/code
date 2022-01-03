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
void *malloc_hook(size_t size, const char *file, int line) {

	void *p = malloc(size);

	char buff[128] = {0};
	sprintf(buff, "./mem/%p.mem", p);

	FILE *fp = fopen(buff, "w");
	fprintf(fp, "[+%s:%d] --> addr:%p, size:%ld\n", file, line, p, size);
	fflush(fp);

	fclose(fp);

	return p;
}

void free_hook(void *p, const char *file, int line) {

	char buff[128] = {0};
	sprintf(buff, "./mem/%p.mem", p);

	if (unlink(buff) < 0) { // no exist
		printf("double free: %p\n", p);
		return ;
	}

	free(p);

}
//宏定义不能放到上边，不然会递归死循环
#if 1
#define malloc(size)	malloc_hook(size, __FILE__, __LINE__)
#define free(p)			free_hook(p, __FILE__, __LINE__)
#endif

int main(int argc, char **argv)
{
	void *p1 = malloc(10);
	void *p2 = malloc(10);
	free(p1);
	return 0;
}



