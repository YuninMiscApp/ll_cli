#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <stdlib.h>
#include "commands.h"
#include "multitask.h"

static os_broadcast_t *g_ll_broadcast = NULL;

int main (int argc, char *argv[])
{
	int ret;
	char buf[1024] = {0};
	g_ll_broadcast = os_broadcast_create(BROADCAST_TYPE_SERVER, COMMAND_PORT);
	RETURN_VAL_IF_FAIL(g_ll_broadcast, -1);
	while(1)
	{
		bzero(buf,sizeof(buf));
		ret = os_broadcast_recv(g_ll_broadcast, (unsigned char *)buf, sizeof(buf));
		MLOGD("Recv %d data: %s",ret,buf);
		if(ret > 0)
			os_broadcast_send(g_ll_broadcast, (unsigned char *)"OK", strlen("OK"));
		else
			os_broadcast_send(g_ll_broadcast, (unsigned char *)"FAIL", strlen("FAIL"));
	}
	os_broadcast_destroy(g_ll_broadcast);
	return 0;
}

