#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include "commands.h"
#include "multitask.h"

int main (int argc, char *argv[])
{
	commands_init();
	while(1) 
		sleep(1);
	return 0;
}

