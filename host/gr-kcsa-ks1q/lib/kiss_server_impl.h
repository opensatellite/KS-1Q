/* -*- c++ -*- */
/* 
 * Copyright 2016 KeChuang Space Academy.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_KCSA_KISS_SERVER_IMPL_H
#define INCLUDED_KCSA_KISS_SERVER_IMPL_H

#include <kcsa/kiss_server.h>
#include <netinet/in.h>
#include <list>
#include <vector>
#include <queue>

namespace gr {
  namespace kcsa {

    class kiss_server_impl : public kiss_server
    {
     private:
      enum kiss_state_e { 
      	  kiss_idle,
      	  kiss_type,
      	  kiss_data,
      	  kiss_ctrl
      };
      	
      struct client_s {
      	int fd;
      	std::vector<uint8_t> buf;
      	int len;
      	enum kiss_state_e state;
      };
      
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;
      pmt::pmt_t d_status_port;
      pmt::pmt_t d_ctrl_port;
      boost::thread   *d_thread;
      
      struct sockaddr_in serv_addr;
      std::list<struct client_s> clients;
      std::queue<uint8_t> tx_queue;
      int mtu;
      int tx_len;
      int rx_len;
      int flush_timeout;
      
      void readloop();
      void in_callback(pmt::pmt_t msg);
      void status_callback(pmt::pmt_t msg);
      int  fd_is_valid(int fd);

     public:
      bool d_finished;
      
      kiss_server_impl(char *address, int port, int mtu, int tx_len, int flush_timeout);
      //mtu is maximum kiss frame size. 
      //for csp packet mtu 256, this should be 256*2(FESC) + 2(FEND) + 1(TNC DATA) = 515
      ~kiss_server_impl();
      
     
      bool start();
      bool stop();
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_KISS_SERVER_IMPL_H */


