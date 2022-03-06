#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include "commands.h"
#include "cmd_edit.h"
#include "multitask.h"

struct ll_ctrl_s {
	int res;
};

static os_broadcast_t *g_os_broadcast = NULL;

#define CMD_SEND(_data_,_data_size_) ({ \
	int _ret_ = -1;\
	if((_ret_ = os_broadcast_send(g_os_broadcast, _data_, _data_size_)) < 0) \
		MLOGE("send cmd fail(%d)!!! %s (%d)",_ret_,strerror(errno),errno);\
})

#define CMD_RECV(_data_,_data_size_,_ret_) ({ \
		if((_ret_ = os_broadcast_recv(g_os_broadcast, _data_, _data_size_)) < 0) \
			MLOGE("send cmd fail(%d)!!! %s (%d)",_ret_,strerror(errno),errno);\
		else \
			MLOGM(" %s ",_data_);\
	})


static void print_help(const char *cmd);
/*****************************************************************/

static int cli_cmd_help(ll_ctrl_t *ctrl, int argc, char *argv[])
{
	print_help(argc > 0 ? argv[0] : NULL);
	return 0;
}

static char ** cli_complete_help(const char *str, int pos)
{
	char **res = NULL;

	return res;
}


static int cli_cmd_quit(ll_ctrl_t *ctrl, int argc, char *argv[])
{
	exit(0);
	return 0;
}

static int cli_cmd_history_list(ll_ctrl_t *ctrl, int argc, char *argv[])
{
	cmd_history_list();
	return 0;
}

static int cli_cmd_wifi_connect(ll_ctrl_t *ctrl, int argc, char *argv[])
{
	const char *ssid,*passwd;
	int sec = 0;
	int ret = 0;
	if(argc < 3)
	{
		MLOGE("wifi_connect '\"[ssid]\"' '\"[passwd]\"' [encryption] = connecting WIFI." );
		return -1;
	}
	ssid = argv[0];
	passwd = argv[1];
	sec = atoi(argv[2]);
	MLOGM("wifi_connect %s %s %d",ssid,passwd,sec);
	char buf[1024] = {0};
	snprintf(buf,sizeof(buf),"wifi_connect %s %s %d",ssid,passwd,sec);
	CMD_SEND((unsigned char *)buf, strlen(buf));
	bzero(buf, sizeof(buf));
	CMD_RECV((unsigned char *)buf, sizeof(buf),ret);
	return 0;
}

/*****************************************************************/

static const struct ll_cli_cmd_s ll_cli_commands[] = {
	{ "help", cli_cmd_help, cli_complete_help,ll_cmd_flag_none,"[command] = show usage help" },
	{ "?", cli_cmd_help, cli_complete_help,ll_cmd_flag_none,"[command] = show usage help" },
	{ "quit", cli_cmd_quit, NULL,ll_cmd_flag_none,"= Quit the Terminal application." },
	{ "history", cli_cmd_history_list, NULL,ll_cmd_flag_none,"= command list" },
	{ "wifi_connect", cli_cmd_wifi_connect, NULL,ll_cmd_flag_none,"'\"[ssid]\"' '\"[passwd]\"' [encryption] = connecting WIFI." },
	{ NULL, NULL, NULL, ll_cmd_flag_none, NULL }
};


/*
 * Prints command usage, lines are padded with the specified string.
 */
static void print_cmd_help(const struct ll_cli_cmd_s *cmd, const char *pad)
{
	char c;
	size_t n;

	printf("%s%s ", pad, cmd->cmd);
	for (n = 0; (c = cmd->usage[n]); n++) {
		printf("%c", c);
		if (c == '\n')
			printf("%s", pad);
	}
	printf("\n");
}

int str_starts(const char *str, const char *start)
{
	return strncmp(str, start, os_strlen(start)) == 0;
}

static void print_help(const char *cmd)
{
	int n;
	printf("commands:\n");
	for (n = 0; ll_cli_commands[n].cmd; n++) {
		if (cmd == NULL || str_starts(ll_cli_commands[n].cmd, cmd))
			print_cmd_help(&ll_cli_commands[n], "  ");
	}
}

/*****************************************************************/
#define max_args 10

int tokenize_cmd(char *cmd, char *argv[])
{
	char *pos;
	int argc = 0;

	pos = cmd;
	for (;;) {
		while (*pos == ' ')
			pos++;
		if (*pos == '\0')
			break;
		argv[argc] = pos;
		argc++;
		if (argc == max_args)
			break;
		if (*pos == '"') {
			char *pos2 = strrchr(pos, '"');
			if (pos2)
				pos = pos2 + 1;
		}
		while (*pos != '\0' && *pos != ' ')
			pos++;
		if (*pos == ' ')
			*pos++ = '\0';
	}

	return argc;
}

static int commands_request(struct ll_ctrl_s *ctrl, int argc, char *argv[])
{
	const struct ll_cli_cmd_s *cmd, *match = NULL;
	int count;
	int ret = 0;
	if (argc == 0)
			return -1;
	count = 0;
	cmd = ll_cli_commands;
	while (cmd->cmd) {
		if (strncasecmp(cmd->cmd, argv[0], os_strlen(argv[0])) == 0)
		{
			match = cmd;
			if (strcasecmp(cmd->cmd, argv[0]) == 0) {
				/* we have an exact match */
				count = 1;
				break;
			}
			count++;
		}
		cmd++;
	}
	if (count > 1) {
		MLOGW("Ambiguous command '%s'; possible commands:", argv[0]);
		cmd = ll_cli_commands;
		while (cmd->cmd) {
			if (strncasecmp(cmd->cmd, argv[0],
					   os_strlen(argv[0])) == 0) {
				printf(" %s", cmd->cmd);
			}
			cmd++;
		}
		printf("\n");
		ret = 1;
	} else if (count == 0) {
		MLOGE("Unknown command '%s'\n", argv[0]);
		ret = 1;
	} else {
		ret = match->handler(ctrl, argc - 1, &argv[1]);
	}
	return ret;
}

static void commands_handler(const char *cmd,void *user_data)
{
	char *argv[max_args];
	int argc;
	argc = tokenize_cmd((char *)cmd, argv);
	if (argc)
		commands_request(user_data, argc, argv);
}


int commands_init(void)
{
	ll_ctrl_t *ctrl = NULL;
	g_os_broadcast = os_broadcast_create(BROADCAST_TYPE_CLIENT, COMMAND_PORT);
	RETURN_VAL_IF_FAIL(g_os_broadcast, -1);
	return cmd_edit_init(commands_handler,ctrl);
}



void commands_exit(void)
{
	os_broadcast_destroy(g_os_broadcast);
}


