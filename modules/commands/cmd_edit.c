#include <stdio.h>
#include <sys/types.h>

#include <stdlib.h>
#include "readline/readline.h"
#include "readline/history.h"

#include "cmd_edit.h"
#include "multitask.h"

extern HIST_ENTRY **history_list ();



#define OS_TASK_NAME_CMD_EDIT  "TAKS_CMD_EDIT"

typedef struct cmd_edit_ctx_s {
	cmd_edit_handler_t handler;
	void *user_data;
}cmd_edit_ctx_t;

static void *task_cmd_edit_routine(void *arg)
{
	cmd_edit_ctx_t *ctx = (cmd_edit_ctx_t *)arg;
	RETURN_VAL_IF_FAIL(ctx, NULL);
	char *temp, *prompt;
	int done;

	temp = (char *)NULL;
	prompt = "cli$ ";
	done = 0;

	while (!done)
	{
		temp = readline (prompt);
		/* Test for EOF. */
		if (!temp)
		{
			exit(0);
		}
		/* If there is anything on the line, print it and remember it. */
		if (*temp)
		{
			add_history (temp);
		}
				
		if(ctx->handler) ctx->handler(temp,ctx->user_data);
		
		if(temp) 
			free (temp);
		temp = NULL;
	}
	return NULL;
}



int cmd_edit_init(cmd_edit_handler_t handler,void *user_data)
{
	cmd_edit_ctx_t *ctx = MALLOC(sizeof(cmd_edit_ctx_t));
	RETURN_VAL_IF_FAIL(ctx, -1);
	ctx->handler = handler;
	ctx->user_data = user_data;
	os_task_create(OS_TASK_NAME_CMD_EDIT, 0, 0, task_cmd_edit_routine, (void *) ctx);
	return 0;
}


void cmd_history_list(void)
{
	HIST_ENTRY **list;
	register int i;
	list = history_list ();
	if (list)
	{
		for (i = 0; list[i]; i++)
		fprintf (stderr, "%d: %s\r\n", i, list[i]->line);
	}
}


