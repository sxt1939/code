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

#include <mcheck.h>

int main(int argc, char **argv)
{
	mtrace(); //开启内存使用记录

	void *p1 = malloc(10);
	void *p2 = malloc(20);
	free(p1);

	muntrace(); //取消内存使用记录

	return 0;
}



