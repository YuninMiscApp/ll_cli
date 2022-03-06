#ifndef __CMD_EDIT__H
#define __CMD_EDIT__H

#ifdef  __cplusplus
extern "C" {
#endif


typedef void (*cmd_edit_handler_t)(const char *cmd,void *user_data);

int cmd_edit_init(cmd_edit_handler_t handler,void *user_data);
void cmd_history_list(void);

#ifdef  __cplusplus
}
#endif


#endif


