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

#include <mcheck.h>

int main(int argc, char **argv)
{
	mtrace(); //�����ڴ�ʹ�ü�¼

	void *p1 = malloc(10);
	void *p2 = malloc(20);
	free(p1);

	muntrace(); //ȡ���ڴ�ʹ�ü�¼

	return 0;
}



