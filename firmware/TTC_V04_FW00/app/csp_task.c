#include "csp_task.h"
#include "pkg_cb.h"

#include "rf_tx_task.h"
#include "rf_rx_task.h"

csp_thread_handle_t handle_client;

CSP_DEFINE_TASK( task_server)
{

	/* Create socket without any socket options */
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	/* Bind all ports to socket */
	csp_bind(sock, CSP_ANY);

	/* Create 10 connections backlog queue */
	csp_listen(sock, 10);

	/* Pointer to current connection and packet */
	csp_conn_t *conn;
	csp_packet_t *packet;

	/* Process incoming connections */
	while (1)
	{

		/* Wait for connection, 10000 ms timeout */
		if ((conn = csp_accept(sock, 1000)) == NULL)
			continue;

		/* Read packets. Timout is 100 ms */
		while ((packet = csp_read(conn, 100)) != NULL)
		{
			int dport = csp_conn_dport(conn);

			if (!csp_pkg_callback(dport, packet))
			{
				csp_service_handler(conn, packet);
			}
		}
	}

	/* Close current connection, and handle next */
	csp_close(conn);

}

