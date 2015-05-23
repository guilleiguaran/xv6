#include "types.h"
#include "user.h"
#include "fcntl.h"

int nums[1000000];

void fill(int n)
{
	int i;
	for(i = 0; i < sizeof(nums) / sizeof(nums[0]); ++i)
		nums[i] = n;
}

int check(int n)
{
	int i;
	for(i = 0; i < sizeof(nums) / sizeof(nums[0]); ++i)
		if(nums[i] != n)
			return 1;
	return 0;
}

int main()
{
	int i, pid, fd;
	char content = 'a';
	printf(0, "Test the speed of fork...\n");
	for(i = 0; i < 10; ++i)
	{
		pid = fork();
		if(pid == 0)
		{
			exit();
		}
		else
		{
			pid = wait();
		}
	}
	printf(0, "Finish\n");
	printf(0, "Test the correctness of fork (Copy-On-Write)...\n");
	fill(1);
	pid = fork();
	if(pid == 0)
	{
		fill(0);
		exit();
	}
	else
	{
		pid = wait();
		if(check(1))
			printf(1, "Error in child write\n");
	}

	pid = fork();
	if(pid == 0)
	{
		sleep(200);
		if(check(1))
			printf(1, "Error in parent write\n");
		exit();
	}
	else
	{
		fill(0);
		pid = wait();
	}

	fd = open("cowtest_temp", O_CREATE|O_WRONLY);
	write(fd, &content, 1);
	close(fd);
	content = 'b';

	pid = fork();
	if(pid == 0)
	{
		fd = open("cowtest_temp", O_RDONLY);
		read(fd, &content, 1);
		exit();
	}
	else
	{
		pid = wait();
		if(content != 'b')
			printf(1, "Error in child kernel write\n");
	}

	unlink("cowtest_temp");

	printf(0, "Finish\n");

	exit();
}
