#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include "commands.h"
#include "multitask.h"

int main (int argc, char *argv[])
{
	setenv("TERMINFO","/usr/share/terminfo",1);
	if(0 != commands_init())
	{
		return -1;
	}
	while(1) 
		sleep(1);
	commands_exit();
	return 0;
}

