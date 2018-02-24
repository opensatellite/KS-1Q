#pragma once

extern bool cmd_prompted;
extern bool console_enable;

#define CMD_SUCCESS      0  //success
#define CMD_FIRST_ERROR -1  //first word is wrong
#define CMD_ARGC_ERROR  -2  //number of arguments is error
#define CMD_ARGS_ERROR  -3  //other argument text wrong

int cmd_parse_line(char cmd[]);
int cmd_process(int argc, char **argv);
void cmd_recv();

void show_usage();

int console(int port, uint32_t baud);
void console_off();