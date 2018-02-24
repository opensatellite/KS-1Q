#include "os.h"
#include "binaryobjects.h"
#include "packet.h"
#include "serial_svc.h"
#include "fprint.h"

static bool serial_enable = false;
static uint32_t send_interval = 0;
static volatile uint32_t last_send_us;
static volatile uint64_t packet_counter;
static volatile uint64_t byte_counter;
static packet_session_t serial_session;

uart_config_t serial_uart_config = {
	.USART_BaudRate = 0,
	.USART_WordLength = USART_WordLength_8b,
	.USART_StopBits = USART_StopBits_1,
	.USART_Parity = USART_Parity_No,
	.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
	.USART_HardwareFlowControl = USART_HardwareFlowControl_None
};

int serial(int port, uint32_t baud)
{
	packet_counter = 0;
	byte_counter = 0;
	serial_uart_config.USART_BaudRate = baud;
	uart_init(port, &serial_uart_config);
	int ret = packet_init(&serial_session, port);
	if(ret  < 0)
	{
		//handle this error
		error("[comm]failed to open serial port\r\n");
		serial_enable = false;
		return ret;
	}
	printf("[comm]serial port opened\r\n");
	last_send_us = 0;
	serial_enable = true;
	packet_counter = 0;
	byte_counter = 0;
	return 0;
}

void serial_rate(int rate)
{
	send_interval = 1000000 / rate;
}

void serial_off()
{
	serial_enable = false;
	pakcet_deinit(&serial_session);
	uart_deinit(serial_session.fd);
}

void serial_info()
{
	if(serial_enable) {
		//show: port id, baudrate, databits, parity, stopbits
		printf("[comm]serial link: port=%d, baud=%lu, mode=8N1\r\n",
			 serial_session.fd, serial_uart_config.USART_BaudRate
		);
	} else {
		printf("[comm]serial link: disabled\r\n");
	}
	//packet sent, byte sent, rate limit
	printf("sent: %llu packets, %llu bytes, rate limit %.3f sps\r\n",
		 packet_counter, byte_counter, 1000000.0 / send_interval
	);
}

void serial_send_adcdata(uint32_t timestamp, int adc, int n, uint16_t ovf, float data[])
{
	if(!serial_enable) return;
	uint32_t now = micros();
	if(now - last_send_us < send_interval && send_interval > 0) return;
	uint8_t status_bytes = (n / 8) + (n % 8 ? 1 : 0);
	uint8_t sz = 4 + (4 * n) + status_bytes;
	sys_noirq();
	packet_tx_head(&serial_session, PACKET_ADC_BASE + adc, sz);
	packet_tx(&serial_session, &timestamp, 4);
	packet_tx(&serial_session, data, 4 * n);
	packet_tx(&serial_session, &ovf, status_bytes);
	packet_tx_tail(&serial_session);
	sys_irq();
	last_send_us = now;
	packet_counter++;
	uint64_t total_bytes = 5 + 2 + sz;
	byte_counter += total_bytes;
}

void serial_send_thermocouple(uint32_t timestamp, uint16_t mask, int n, char type[], int8_t ovf[], float temp[])
{
	if(!serial_enable) return;
	sys_noirq();
	int cnt = 0;
	for(int i = 0; i < n; i++)
		if(mask & (1<<i)) cnt++;
	packet_tx_head(&serial_session, PACKET_TC, 4 + 2 + 6*cnt);
	packet_tx(&serial_session, &timestamp, 4);
	packet_tx(&serial_session, &mask, 2);
	for(int i = 0; i < n; i++)
	{
		packet_tx(&serial_session, &type[i], 1);
		packet_tx(&serial_session, &ovf[i],  1);
		packet_tx(&serial_session, &temp[i], 4);
	}
	packet_tx_tail(&serial_session);
	sys_irq();
	packet_counter++;
	uint64_t total_bytes = 5 + 2 + 4 + (2+6*cnt);
	byte_counter += total_bytes;
}

void serial_send_mark(int id, uint64_t s, uint32_t us, char str[])
{
	if(!serial_enable) return;
	int str_l = 0;
	if(str) str_l = strlen(str);
	str_l = str_l > 200 ? 200 : str_l;
	sys_noirq();
	packet_tx_head(&serial_session, PACKET_MARK, 14 + str_l);
	packet_tx(&serial_session, &id, 2);
	packet_tx(&serial_session, &s,  8);
	packet_tx(&serial_session, &us, 4);
	if(str) packet_tx(&serial_session, str, str_l);
	packet_tx_tail(&serial_session);
	sys_irq();
	packet_counter++;
	uint64_t total_bytes = 5+2+(14 + str_l);
	byte_counter += total_bytes;
}
