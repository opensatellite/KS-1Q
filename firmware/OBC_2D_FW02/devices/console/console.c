#include "os.h"
#include "console.h"

#include "main.h"
#include "thermocouple.h"	//algotest
//#include "adc_reader.h"
//#include "thermocouple_reader.h"
#include "fprint.h"
#include "sdcard.h"
#include "mark.h"
#include "serial_svc.h"

int console_uart;
bool cmd_prompted;
bool console_enable = false;

/*
int putchar(int c)
{
	if(console_enable)
		uart_putc(console_uart,(char)c);
	return c;
}

int getchar(void)
{
	if(!console_enable)
		return -1;
	int c = uart_getc(console_uart);
	return c;
}*/


uart_config_t console_uart_config = {
		.USART_BaudRate = 0,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None
};

int console(int port, uint32_t baud)
{
	console_uart = port;
	console_uart_config.USART_BaudRate = baud;
	uart_init(console_uart,&console_uart_config);
	int ret = uart_open(console_uart,O_RDWR);
	if(ret  < 0)
	{
		//handle this error
		error("[comm]failed to open console port\r\n");
		console_enable = false;
		return ret;
	}
	console_enable = true;
	return 0;
}

void console_off()
{
	console_enable = false;
	uart_close(console_uart);
	uart_deinit(console_uart);
}

int cmd_to_argv(char cmd[],char *argv[],int n)
{
	if(argv == 0) return 0;
	if(n == 0) return 0;
	char *p = cmd;
	int length = 0;
	while(*p) 
	{
		//replace
		if(*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') *p = '\0';
		p++;length++;
	}
	
	int argc = 0;
	while(argc < n)
	{
		while(*cmd == '\0') 
		{
			cmd++;length--;if(length <= 0) return argc;
		}
		argv[argc++] = cmd;
		while(*cmd)
		{
			cmd++;length--;if(length <= 0) return argc;
		}
	}
	return argc;
}

int str_match_list(const char str[],const char *list[],int n)
{
	for(int i=0;i<n;i++)
		if(strcmp(str,list[i]) == 0)
			return i;
	return -1;//not found
}

bool arg_parse_bool(char str[])
{
	bool cmd;
	if(strcmp(str,"true") == 0) cmd = true;
	else if(strcmp(str,"enable") == 0) cmd = true;
	else if(strcmp(str,"on") == 0) cmd = true;
	else cmd = false;
	return cmd;
}

void cmd_recv()
{
	if(console_enable)
	{
		int nbytes;
		nbytes = uart_available(console_uart);
		if(nbytes > 0)
		{
			//Default cli parser
			while(uart_can_readline(console_uart))
			{		
				char cmdbuf[80];
				uart_readline(console_uart,cmdbuf,80);
				cmd_parse_line(cmdbuf);
				cmd_prompted = false;
			}
		}
		
		if(!cmd_prompted)
		{
			printf("daq>");
			cmd_prompted = true;
		}
	}
}
	
int cmd_parse_line(char cmd[])
{
	char *argv[16];
	int argc = cmd_to_argv(cmd,argv,16);
	int ret = 0;
	if(argc != 0)
	{
		ret = cmd_process(argc,argv);
		switch(ret)
		{
		case 0:break;//ok
		case -1:printf("no such command\r\n\r\n");show_usage();break;
		case -2:printf("number of args wrong\r\n");break;
		case -3:printf("wrong arguments\r\n");break;
		default:printf("unknown error %d\r\n",ret);
		}
		printf("%d|",ret);
	}
	return ret;
}

int cmd_process(int argc, char **argv)
{
	if(argc == 0) return CMD_ARGC_ERROR; 
	const char* cmd_list[] = {
		"help", "reboot", "adc", "tc", "serial", "modbus", "run", "stop",
		"mark", "algotest", "sdcard", "exit", "cat", "edit", "rm", "exec",
		"ls", "cd", "cwd", "mkdir", "flush"
	};
	int idx = str_match_list(argv[0],cmd_list,NUMBER_OF(cmd_list));
	switch(idx)
	{
	case 0://help
		show_usage();
		return CMD_SUCCESS;
	case 1://reboot
		//close sdcard log files
		sdcard_off();
	
		reboot();
		return CMD_SUCCESS;
		/*
	case 2://adc
		{
			if(argc >= 2)
			{
				if(!strcmp(argv[1], "run"))
				{
					//adc_run();
					return CMD_SUCCESS;
				}
				else if(!strcmp(argv[1], "stop"))
				{
					//adc_stop();
					return CMD_SUCCESS;
				}
				else
				{
					int32_t cnt = strtol(argv[1], NULL, 10);
					if(cnt <= 0) return CMD_ARGS_ERROR;
					//show_adc(cnt);
					return CMD_SUCCESS;
				}
			}
			else if(argc == 1)
			{
				//show_adc(-1);
				return CMD_SUCCESS;
			}
			else return CMD_ARGC_ERROR;
		}
	case 3://thermocouple
		{
			if(argc == 4)
			{
				//thermocouple set [channel] [type]
				if(!strcmp(argv[1],"set"))
				{
					//check type
					if(strlen(argv[3]) != 1)
						return CMD_ARGS_ERROR;
					//check channels
					char *s = argv[2];
					while(*s)
					{
						if(!isdigit(*s)) return CMD_ARGS_ERROR;
						s++;
					}
					int ch = strtol(argv[2], NULL, 10);
					if(ch < 0 || ch >= 16)
						return CMD_ARGS_ERROR;
					//apply settings
					int ret = thermocouple_set(ch, argv[3][0]);
					if(ret == THERMOCOUPLE_NO_CHANNEL)
					{
						printf("No such channel");
						return -4;
					}
					else if(ret < 0)
					{
						printf("ADC %d not in differential mode\r\n", -ret);
						return -4;
					}
					return CMD_SUCCESS;
				}
				return CMD_ARGS_ERROR;
			}
			else if(argc == 1)
			{
				show_thermocouple();
				//show measurements
			}
			else return CMD_ARGC_ERROR;
		}
		return CMD_SUCCESS;
		*/
	case 4://serial
		{
			if(argc < 2) 
			{
				serial_info();
				return CMD_SUCCESS;
			}
			const char *cmd_list[] = {"info", "rate", "off"};
			int idx = str_match_list(argv[1], cmd_list, NUMBER_OF(cmd_list));
			switch(idx)
			{
			case 0://info
				serial_info();
				return CMD_SUCCESS;
			case 1://rate
				{
					int32_t rate = strtol(argv[2], NULL, 10);
					if(rate < 1 || rate > 24000)
						return CMD_ARGS_ERROR;
					serial_rate(rate);
					return CMD_SUCCESS;
				}
			case 2://off
				serial_off();
				return CMD_SUCCESS;
			default://baud [rate] [port]
				{
					uint32_t baud = strtoul(argv[1], NULL, 10);
					if(baud)
					{
						if(argc == 2)//serial baud
						{
							serial_rate(INFINITY);
							return serial(SERIAL_PORT, baud);
						}
						else if(argc == 3)//serial baud rate
						{
							int32_t rate = strtol(argv[2], NULL, 10);
							if(rate < 1 || rate > 24000)
								return CMD_ARGS_ERROR;
							serial_rate(rate);
							return serial(SERIAL_PORT, baud);
						}
						else if(argc == 4)
						{
							int32_t rate = strtol(argv[2], NULL, 10);
							if(rate < 1 || rate > 24000)
								return CMD_ARGS_ERROR;
							int32_t port = strtol(argv[3], NULL, 10);
							if(port < 0 || port >= UART_PORTS)
								return CMD_ARGS_ERROR;
							serial_rate(rate);
							return serial(port, baud);
						}
						else return CMD_ARGC_ERROR;
					}
					return CMD_ARGS_ERROR;
				}
			}
		}
	case 5://modbus
		{
			if(argc < 2) 
			{
				serial_info();
				return CMD_SUCCESS;
			}
			//baud [addr] [port]
			//info, addr, off
		}
	case 6://run
		//daq_run();
		return CMD_SUCCESS;
	case 7://stop
		//daq_stop();
		return CMD_SUCCESS;
	case 8://mark
		{
			if(argc == 2)
			{
				time_mark((int)strtol(argv[1], NULL, 10));
				return CMD_SUCCESS;
			}
			else if(argc == 1)
			{
				time_mark(0);
				return CMD_SUCCESS;
			}
			else return CMD_ARGC_ERROR;
		}
	case 9://algotest
		thermocouple_algo_test();
		return CMD_SUCCESS;
	case 10://sdcard
		{
			if(argc < 2) 
			{
				sdcard_info();
				return CMD_SUCCESS;
			}
			const char* op_list[] = {"on", "off", "format", "diskinfo", "fsinfo", "rate", "flush", "info"};
			int idx = str_match_list(argv[1], op_list, NUMBER_OF(op_list));
			switch(idx)
			{
			case 0:
				sdcard_reload();
				return CMD_SUCCESS;
			case 1:
				sdcard_off();
				return CMD_SUCCESS;
			case 2://format
			{
				long n;
				if(argc == 3)
					n = strtol(argv[2], NULL, 10);
				else
					n = 0;
				sdcard_format(n);
				return CMD_SUCCESS;
			}
			case 3:
				sdcard_diskinfo();
				return CMD_SUCCESS;
			case 4:
				sdcard_fsinfo();
				return CMD_SUCCESS;
			case 5:
				
				return CMD_SUCCESS;
			case 6:
				return CMD_SUCCESS;
			case 7:
				sdcard_info();
				return CMD_SUCCESS;
			default:
				return CMD_ARGS_ERROR;
			}
		}
	case 11://exit
		console_enable = false;//just disable stdio, not deinit
		return CMD_SUCCESS;
	case 12://cat
		if(argc < 2) return CMD_ARGC_ERROR;
		for(int i=1;i<argc;i++)
			sdcard_reader(argv[i]);
		return CMD_SUCCESS;
	case 13://edit
		if(argc < 2) return CMD_ARGC_ERROR;
		sdcard_writer(argv[1]);
		return CMD_SUCCESS;
	case 14://rm
		if(argc < 2) return CMD_ARGC_ERROR;
		for(int i=1;i<argc;i++)
			sdcard_remove(argv[i]);
		return CMD_SUCCESS;
	case 15://exec
		if(argc < 2) return CMD_ARGC_ERROR;
		for(int i=1;i<argc;i++)
			sdcard_execute(argv[i]);
		return CMD_SUCCESS;
	case 16://ls
		{
			if(argc < 2)
			{
				char cwd[_MAX_LFN];
				f_getcwd(cwd, _MAX_LFN);
				sdcard_ls(cwd);
			}
			else
			{
				for(int i=1;i<argc;i++)
					sdcard_ls(argv[i]);
			}
			return CMD_SUCCESS;
		}
	case 17://cd
		{
			if(argc < 2)
				sdcard_cd("/");
			else
				sdcard_cd(argv[1]);
			return CMD_SUCCESS;
		}
	case 18://cwd
		sdcard_showcwd();
		return CMD_SUCCESS;
	case 19://mkdir
		{
			if(argc < 2)
				return CMD_ARGS_ERROR;
			for(int i=1;i<argc;i++)
				sdcard_mkdir(argv[i]);
			return CMD_SUCCESS;
		}
	case 20://flush
		{
			return CMD_SUCCESS;
		}
	default:
		if(strlen(argv[0]) == 0) 
			return CMD_SUCCESS;//no command
		return CMD_FIRST_ERROR;
	}
}

void show_usage()
{
	printf("usage:\r\n");
	printf("    run                         start daq\r\n");											//ok
	printf("    stop                        stop daq\r\n");												//ok
	
	printf("    ls   [path] [...]           list files\r\n");											//ok
	printf("    cd   [path]                 change directory\r\n");										//ok
	printf("    cwd                         show current working directory\r\n");						//ok
	printf("    mkdir [dir] [...]           create new directory\r\n");									//ok
	printf("    cat  [file] [...]           show file content\r\n");									//ok
	printf("    edit [file]                 write stdin to file\r\n");									//ok
	printf("    rm   [file] [...]           remove file\r\n");											//ok
	printf("    exec [file] [...]           execute script file\r\n");									//ok
	
	printf("    adc                         show adc status and input voltage\r\n");					//ok
	printf("    adc [n]                     show adc for n times\r\n");									//ok
	printf("    adc run                     start adc\r\n");											//ok
	printf("    adc stop                    stop adc\r\n");												//ok
	printf("    tc                          show thermocouple status and readings\r\n");				//ok
	printf("    tc [n]                      show thermocouple for n times\r\n");						//ok
	printf("    mark [id]                   add time mark, id=[-32768~+32767], default=0\r\n");			//ok
	
	printf("    serial info                 show serial info\r\n");
	printf("    serial baud [rate] [port]   start serial data streaming\r\n");
	printf("                                baud=[1200-10500000], rate=[1-24000], port=[0-2]\r\n");
	printf("    serial rate [rate]          set serial streaming rate limit to [1-24000]sps\r\n");
	printf("    serial off                  serial data streaming off\r\n");

	printf("    modbus baud [addr] [port]   start modbus interface\r\n");
	printf("                                baud=[1200-1000000], addr=[1-247], port=[0-2]\r\n");
	printf("    modbus addr [addr]          set modbus address to [1-247]\r\n");
	printf("    modbus off                  modbus interface off\r\n");	

	printf("    sdcard info                 show sdcard hardware and filesystem info\r\n");				//ok
	printf("    sdcard on                   start sdcard or reload sdcard\r\n");						//ok
	printf("    sdcard off                  force sdcard offline after all data saved\r\n");			//ok
	printf("    sdcard format               format sdcard\r\n");										//ok
	printf("    sdcard rate [rate]          set sdcard record rate limit to [1-24000]sps\r\n"); 		//ok
	printf("    sdcard flush                write all buffered data to sdcard\r\n");                	//ok

	printf("    reboot                      system reset\r\n");											//ok
	printf("    exit                        exit command prompt\r\n");									//ok
	printf("    help                        show usage info\r\n");										//ok
}

