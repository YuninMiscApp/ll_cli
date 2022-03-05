#ifndef __COMMANDS__H
#define __COMMANDS__H

#ifdef  __cplusplus
extern "C" {
#endif

enum ll_cmd_flags {
	ll_cmd_flag_none		= 0x00,
	ll_cmd_flag_sensitive   = 0x01
};

typedef struct ll_ctrl_s ll_ctrl_t;

typedef struct ll_cli_cmd_s {
	const char *cmd;
	int (*handler)(ll_ctrl_t *ctrl, int argc, char *argv[]);
	char ** (*completion)(const char *str, int pos);
	enum ll_cmd_flags flags;
	const char *usage;
}ll_cli_cmd_t;



int commands_init(void);


#ifdef  __cplusplus
}
#endif


#endif


