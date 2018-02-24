/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <sys/types.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>

#include "kiss_server_impl.h"
#include <algorithm> 
#include <queue>

using namespace boost::posix_time;

namespace gr {
  namespace kcsa {
  
#define	FEND	0xC0
#define	FESC	0xDB
#define	TFEND	0xDC
#define	TFESC	0xDD
#define TNC_DATA	0x00
#define TNC_CTRL	0x0c


#define CCSDS_TYPE_UP_IP		2
#define CCSDS_TYPE_DOWN_IP		5
#define CCSDS_TYPE_SHIFT		4

    kiss_server::sptr
    kiss_server::make(char *address, int port, int mtu, int tx_len, int flush_timeout)
    {
      return gnuradio::get_initial_sptr
        (new kiss_server_impl(address, port, mtu, tx_len, flush_timeout));
    }

    /*
     * The private constructor
     */
    kiss_server_impl::kiss_server_impl(char *address, int port, int _mtu, int _tx_len, int _flush_timeout)
      : gr::block("kiss_server",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int))),
              mtu(_mtu),
              tx_len(_tx_len),
              flush_timeout(_flush_timeout)
    {
    	d_in_port = pmt::mp("in");
    	d_out_port = pmt::mp("out");
    	d_status_port = pmt::mp("status");
    	d_ctrl_port = pmt::mp("ctrl");
    	
      	message_port_register_in(d_in_port);
      	message_port_register_in(d_status_port);
      	set_msg_handler(d_in_port, boost::bind(&kiss_server_impl::in_callback, this ,_1) );
      	set_msg_handler(d_status_port, boost::bind(&kiss_server_impl::status_callback, this ,_1) );
      	
      	message_port_register_out(d_out_port);
      	message_port_register_out(d_ctrl_port);
    
    	//convert address string to socket address
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(address);
        serv_addr.sin_port = htons(port); 	
    }

    /*
     * Our virtual destructor.
     */
    kiss_server_impl::~kiss_server_impl()
    {
    	stop();
    }

    bool kiss_server_impl::start()
    {
      d_finished = false;
      d_thread = new boost::thread(boost::bind(&kiss_server_impl::readloop, this));
      return true;
    }

    bool kiss_server_impl::stop()
    {
      d_finished = true;
      d_thread->join();
      delete d_thread;
      return true;
    }

    void kiss_server_impl::readloop()
    {
      //create server
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if(sockfd < 0)
	  {
	      std::cerr << "kiss_server: socket create failed" << std::endl;
		  exit(errno);
		  return;
	  }
	  
	  // set SO_REUSEADDR on a socket to true (1):
      int optval = 1;
      setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	  int ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	  if(ret < 0) 
	  {
          std::cerr << "kiss_server: error on binding " << errno << std::endl;
          exit(ret);
          return;
      }  
      fcntl(sockfd, F_SETFL, fcntl(sockfd,F_GETFL,0) | O_NONBLOCK);
        
      ret = listen(sockfd, 100);
      if(ret < 0)
      {
      	  std::cerr << "kiss_server: failed to listen " << errno << std::endl;
      	  //exit(ret);
      	  return;
      }
      
      ptime last_tx_time = microsec_clock::local_time();
      
      while(!d_finished) {
      	struct sockaddr_in cli_addr;
        socklen_t cli_len;
        
        //check new connections
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
        if(newsockfd >= 0) //new connection
        {
        	struct client_s new_client;
        	new_client.fd = newsockfd;
        	new_client.buf = std::vector<uint8_t>(mtu);
        	new_client.state = kiss_idle;
        	clients.push_back(new_client);
        	
        	fcntl(new_client.fd, F_SETFL, fcntl(sockfd,F_GETFL,0) | O_NONBLOCK);
        	
        	int ret = getsockname(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
        	if(ret >= 0)
        	{
        		char client_ip[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(cli_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        		std::cout << "kiss_server: new connection from " << client_ip << std::endl;
        	}
        }
        if(newsockfd < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        {
        	std::cerr << "kiss_server: failed to accept " << errno << std::endl;
        }
        
        bool have_data = false;
        //check clients data
        for (std::list<struct client_s>::iterator it = clients.begin(), end = clients.end(); it != end;) {
        	bool linklost = false;
        	while(1) {
        		//read a byte
        		unsigned char x;
				if(!fd_is_valid(it->fd)) continue;
			    int ret = read(it->fd, &x, 1);
				if(ret == 0) {
					//connection lost
					//std::cout << "kiss_server: connection " << it->fd << " read zero" << std::endl;
					//remove element
					linklost = true;
					break;
				} else if(ret > 0) {
					//recv data
					//std::cout << "recv: " << it->fd << " data " << x << std::endl;
					//-------process kiss packet here
					switch(it->state) {
						case kiss_idle:
							if(x == FEND) {
								it->buf[0] = x;//save data
								it->len = 1;
								it->state = kiss_type;
							} else {
								it->len = 0;
							}
							break; 
						case kiss_type:
							if(x == TNC_DATA) {
								it->buf[it->len++] = x;
								it->state = kiss_data;
							} else if(x == TNC_CTRL) {
								it->buf[it->len++] = x;
								it->state = kiss_ctrl;
							} else {
								it->state = kiss_idle;
							} 
							break;
						case kiss_data:
							if(it->len == it->buf.size())	//buffer full
							{
								//drop packet
								it->state = kiss_idle;	
							}
							it->buf[it->len++] = x;
							if(x == FEND) {
								//enqueue message
								for(int i = 0; i < it->len; i++)
									tx_queue.push(it->buf[i]);
								//std::cout << "data: " << it->len << std::endl;
								it->state = kiss_idle;
							}
							break;
						case kiss_ctrl:
							if(it->len == it->buf.size())	//buffer full
							{
								//drop packet
								it->state = kiss_idle;	
							}
							it->buf[it->len++] = x;
							if(x == FEND) {
								//kiss decode
								std::vector<uint8_t> msg;
								bool escaped = false;
								for(int i = 2; i < it->len - 1; i++) {
									if(it->buf[i] == FESC) escaped = true;
									else {
										if(escaped) {
											if(it->buf[i] == TFESC)
												msg.push_back(FESC);
											else if(it->buf[i] == TFEND)
												msg.push_back(FEND);
											escaped = false;
										} else {
											msg.push_back(it->buf[i]);
										}
									}
								}
								pmt::pmt_t pmt_msg = pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg.size(), msg));
								message_port_pub(d_ctrl_port, pmt_msg);
								//std::cout << "ctrl msg: " << pmt_msg << std::endl;
								it->state = kiss_idle;
							}
							break;
						default: it->state = kiss_idle;
					}
					have_data = true;
				} else {
					//error occurred
					if(errno == EAGAIN || errno == EWOULDBLOCK) {
						//no data arrived
						break;
					} else {
						//report this err
						std::cerr << "kiss_server: connection " << it->fd << " read error " << errno << std::endl;						
					}//if(errno == ...)
				}//if(ret == 0)
			}//while(1)
			if(linklost)
			{
				std::list<struct client_s>::iterator old_it = it;
				std::cout << "kiss_server: connection " << old_it->fd << " closed" << std::endl;
				it++;
				clients.erase(old_it);
			}
			else it++;
        }
        
        //tx queue pub
        while(tx_queue.size() >= tx_len) {
        	std::vector<uint8_t> msg(tx_len);
        	for(int i = 0; i < tx_len; i++) {
        		msg[i] = tx_queue.front();
        		tx_queue.pop();
        	}
        	pmt::pmt_t pmt_msg = pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg.size(), msg));
			message_port_pub(d_out_port, pmt_msg);
			//std::cout << pmt_msg;
			last_tx_time = microsec_clock::local_time();
        }
        
        //std::cout << microsec_clock::local_time() << "," << last_tx_time << " , " << milliseconds(flush_timeout) << std::endl;
        if(microsec_clock::local_time() - last_tx_time > milliseconds(flush_timeout)
         && tx_queue.size() > 0) {
        	std::vector<uint8_t> msg(tx_len);
        	int q_len = tx_queue.size();
        	for(int i = 0; i < q_len; i++) {
        		msg[i] = tx_queue.front();
        		tx_queue.pop();
        	}
        	for(int i = q_len; i < tx_len; i++)
        		msg[i] = FEND;
        	pmt::pmt_t pmt_msg = pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg.size(), msg));
			message_port_pub(d_out_port, pmt_msg);
			//std::cout << pmt_msg;
			last_tx_time = microsec_clock::local_time();
        }
        
        //do not run too fast
        if(!have_data) {
          boost::this_thread::sleep(milliseconds(flush_timeout));
        }
      }
      //close all connections when exit
      std::cout << "exiting" << std::endl;
      /*
      for (std::list<struct client_s>::iterator it = clients.begin(), end = clients.end(); it != end;) {
      	close(it->fd);
      }
      */
      clients.clear(); 
      close(sockfd);    
    }
    
    int kiss_server_impl::fd_is_valid(int fd)
	{
    	return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
	}
    
    void kiss_server_impl::in_callback(pmt::pmt_t msg)
    {
    	pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t data(pmt::cdr(msg));
		std::vector<uint8_t> u8data = pmt::u8vector_elements(data);
		
		//check packet type
		uint8_t type = u8data[2] >> CCSDS_TYPE_SHIFT;
		if(type != CCSDS_TYPE_DOWN_IP) {
			std::cout << "wrong packet type " << type << std::endl;
			return;
		}
		//skip spacecraft id and version
		uint16_t spacecraft_id = u8data[0] | u8data[1] << 8;
		uint8_t version = u8data[2] & 0x0f;
		std::cout << "spacecraft id: " << spacecraft_id << ", version: " << (int)version << std::endl;
		
		for (std::list<struct client_s>::iterator it = clients.begin(), end = clients.end(); it != end; ++it) {
			if(!fd_is_valid(it->fd)) continue;
			
			//send data to clients
			int ret = write(it->fd, u8data.data() + 3, u8data.size() - 3);
			if(ret < 0) {
				std::cerr << "kiss_server: connection " << it->fd << " write error " << errno << std::endl;
				std::cerr << "kiss_server: connection " << it->fd << " dropped" << std::endl;
				close(it->fd);
			} else if(ret < u8data.size() - 3) { // 0 <= ret < u8data.size()
				std::cout << "kiss_server: connection " << it->fd << " want " << u8data.size() << " written " << ret << ", buffer full" << std::endl;
				std::cout << "kiss_server: connection " << it->fd << " dropped" << std::endl;
				close(it->fd);
			}
		}
    }
    
    void kiss_server_impl::status_callback(pmt::pmt_t msg)
    {
    	pmt::print(msg);
    }

  } /* namespace kcsa */
} /* namespace gr */


