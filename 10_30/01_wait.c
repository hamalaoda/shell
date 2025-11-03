/* ************************************************************************
 *       Filename:  01_wait.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2025年10月30日 19时43分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(int argc, char const *argv[])
{
	int pid = fork();
	if(pid < 0 )
	{
		printf("shibaichuangjian");
	}
	else if (pid == 0)
    {
		for(int i = 0 ; i<10; i++)
		{
			printf("zijingcheng%u zhengzaizhixingdi%d ci\n",getpid(),i);
			sleep(1);
		}
	}
	else if (pid >0)
	{
		printf("fu\n");
		wait(NULL);
		printf("fuhuishoule%u\n",pid);
	}
	return 0;
}



