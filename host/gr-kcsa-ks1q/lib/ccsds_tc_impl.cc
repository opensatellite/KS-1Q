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
#include "ccsds_tc_impl.h"

namespace gr {
  namespace kcsa {

/*
KCSA PLOP-2
Satellite uplink operation procedure
1.  turn on transmitter
2.  uplink_sine: send sine wave for 100+ms
3.  uplink_acq:  send start sequence for 32+ times (64+ octets)
4.  uplink_idle: wait for data packet, send 0x55 (alternating 0 and 1) while waiting
5.  uplink_run:  get a IO/CTRL/SESSION packet or a IP packet
6.  send start sequence
7.  send 3 cltu's for io/ctrl/session packet, or send 32 clut's for data packet
    MSB first, cltu parity inversed
8.  send tail sequence
9.  uplink_idle: wait for data packet, send 0x55 (alternating 0 and 1) while waiting
10. uplink_off:  send sine wave for 100+ms
11. turn off transmitter
*/

#define CCSDS_TYPE_UP_IP		2
#define CCSDS_TYPE_DOWN_IP		5
#define CCSDS_TYPE_SHIFT		4

//telecommand randomizer sequence
static uint8_t pn9[] = {
	0xff,0x39,0x9e,0x5a,0x68,0xe9,0x06,0xf5,
	0x6c,0x89,0x2f,0xa1,0x31,0x5e,0x08,0xc0,
	0x52,0xa8,0xbb,0xae,0x4e,0xc2,0xc7,0xed,
	0x66,0xdc,0x38,0xd4,0xf8,0x86,0x50,0x3d,
	0xfe,0x73,0x3c,0xb4,0xd1,0xd2,0x0d,0xea,
	0xd9,0x12,0x5f,0x42,0x62,0xbc,0x11,0x80,
	0xa5,0x51,0x77,0x5c,0x9d,0x85,0x8f,0xda,
	0xcd,0xb8,0x71,0xa9,0xf1,0x0c,0xa0,0x7b,
	0xfc,0xe6,0x79,0x69,0xa3,0xa4,0x1b,0xd5,
	0xb2,0x24,0xbe,0x84,0xc5,0x78,0x23,0x01,
	0x4a,0xa2,0xee,0xb9,0x3b,0x0b,0x1f,0xb5,
	0x9b,0x70,0xe3,0x53,0xe2,0x19,0x40,0xf7,
	0xf9,0xcc,0xf2,0xd3,0x47,0x48,0x37,0xab,
	0x64,0x49,0x7d,0x09,0x8a,0xf0,0x46,0x02,
	0x95,0x45,0xdd,0x72,0x76,0x16,0x3f,0x6b,
	0x36,0xe1,0xc6,0xa7,0xc4,0x32,0x81,0xef,
	0xf3,0x99,0xe5,0xa6,0x8e,0x90,0x6f,0x56,
	0xc8,0x92,0xfa,0x13,0x15,0xe0,0x8c,0x05,
	0x2a,0x8b,0xba,0xe4,0xec,0x2c,0x7e,0xd6,
	0x6d,0xc3,0x8d,0x4f,0x88,0x65,0x03,0xdf,
	0xe7,0x33,0xcb,0x4d,0x1d,0x20,0xde,0xad,
	0x91,0x25,0xf4,0x26,0x2b,0xc1,0x18,0x0a,
	0x55,0x17,0x75,0xc9,0xd8,0x58,0xfd,0xac,
	0xdb,0x87,0x1a,0x9f,0x10,0xca,0x07,0xbf,
	0xce,0x67,0x96,0x9a,0x3a,0x41,0xbd,0x5b,
	0x22,0x4b,0xe8,0x4c,0x57,0x82,0x30,0x14,
	0xaa,0x2e,0xeb,0x93,0xb0,0xb1,0xfb,0x59,
	0xb7,0x0e,0x35,0x3e,0x21,0x94,0x0f,0x7f,
	0x9c,0xcf,0x2d,0x34,0x74,0x83,0x7a,0xb6,
	0x44,0x97,0xd0,0x98,0xaf,0x04,0x60,0x29,
	0x54,0x5d,0xd7,0x27,0x61,0x63,0xf6,0xb3,
	0x6e,0x1c,0x6a,0x7c,0x43,0x28,0x1e,
};

static const uint8_t bch64_p[] = {
	0xc4,0x62,0xf4,0x7a,0xf8,0x7c,0x3e,
	0xda,0xa8,0x54,0x2a,0xd0,0x68,0x34,
	0x1a,0xc8,0x64,0x32,0xdc,0x6e,0xf2,
	0xbc,0x5e,0xea,0xb0,0x58,0x2c,0x16,
	0xce,0xa2,0x94,0x4a,0xe0,0x70,0x38,
	0x1c,0x0e,0xc2,0xa4,0x52,0xec,0x76,
	0xfe,0xba,0x98,0x4c,0x26,0xd6,0xae,
	0x92,0x8c,0x46,0xe6,0xb6,0x9e,0x8a,
};

    ccsds_tc::sptr
    ccsds_tc::make(uint16_t spacecraft_id, uint8_t version, int decim, bool randomizer, int sine_samples, int acq_samples)
    {
      return gnuradio::get_initial_sptr
        (new ccsds_tc_impl(spacecraft_id, version, decim, randomizer, sine_samples, acq_samples));
    }

    /*
     * The private constructor
     */
    ccsds_tc_impl::ccsds_tc_impl(uint16_t _spacecraft_id, uint8_t _version, int decim, bool _randomizer, int _sine_samples, int _acq_samples) : gr::sync_decimator("ccsds_tc",
        gr::io_signature::make(1, 1, sizeof(gr_complex)),
        gr::io_signature::make(1, 1, sizeof(unsigned char)), decim),
   		spacecraft_id(_spacecraft_id),
   		version(_version),
        randomizer(_randomizer),
        sine_samples(_sine_samples),
        acq_samples(_acq_samples),
        bufpos(0),
        buflen(0)
    {
    	d_pdu_port = pmt::mp("pdu");
    	message_port_register_in(d_pdu_port);
    	set_msg_handler(d_pdu_port, boost::bind(&ccsds_tc_impl::pdu_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    ccsds_tc_impl::~ccsds_tc_impl()
    {
    	
    }

	void ccsds_tc_impl::pdu_callback(pmt::pmt_t msg)
	{
		pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t data(pmt::cdr(msg));
		
		//get a message, push into queue	
		
		size_t msg_len;
		const uint8_t* bytes_in = pmt::u8vector_elements(data, msg_len);
		if(msg_len > payload_bytes)//data
		{
			std::cerr << "ccsds_tc: msg size " << msg_len;
			std::cerr << " , truncate to " << payload_bytes << std::endl;
			msg_len = payload_bytes;
		}	
		std::vector<uint8_t> x(up_ip_bytes);
		memcpy(x.data(), &spacecraft_id, 2);
		x[2] = (CCSDS_TYPE_UP_IP << CCSDS_TYPE_SHIFT) | version;
		memcpy(x.data() + 3, bytes_in, msg_len);
		memset(x.data() + 3 + msg_len, 0, up_ip_bytes - 3 - msg_len);
		q.push(x);
	}

    int
    ccsds_tc_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      unsigned char *out = (unsigned char *) output_items[0];

      // Do <+signal processing+>
      for(int i = 0; i < noutput_items; i++)
      {
      	  if(bufpos < buflen) {
      	  	*out++ = buf[bufpos++];
      	  } else {
      	  switch(stat)
      	  {
      	    	case plop_sine:
      	    	if(sine_samples > 0) {
      	  			*out++ = 0x00;
      	  			sine_count++;
      	  			if(sine_count >= sine_samples) {
      	  				stat = plop_acq;
    					//std::cout << "stat: PLOP acq" << std::endl;
      	  				acq_count = 0;
      	  			}
      	  			break;
      	  		}
      	  		else stat = plop_acq;
      	  		case plop_acq:
      	  			*out++ = 0x55;
      	  			acq_count++;
      	  			//std::cout << "acq count: " << acq_count << std::endl;
      	  			if(acq_count >= acq_samples) {
      	  				stat = plop_idle;
    					//std::cout << "stat: PLOP idle" << std::endl;
      	  			}
      	  		case plop_off:
      	  			*out++ = 0x55;
      	  		break;
      	  		case plop_idle:
      	  		{
      	  			//now buf runs out, it's safe to clear buffer
      	  			//make cltus and fill into buf
      	  			make_cltus();
      	  			
      	  			//if there is new message, transmit them
      	  			if(bufpos < buflen) {
      	  		//		add_item_tag(0, nitems_written(0) + i,pmt::mp("trig"),pmt::from_double(1));
      	  				*out++ = buf[bufpos++];
      	  			} else {
      	  				*out++ = 0x55;
      	  				//std::cout << "work: idle" << std::endl;
      	  			}
      	  		}
      	  		break;
      	  		default: 
      	  			on();
      	  			*out++ = 0x00;
      	  	}
      	  }
      }
      //std::cout << "stat now: " << stat << std::endl;

      // Tell runtime system how many output items we produced.
      //std::cout << "noutput_items: " << noutput_items << std::endl;
      return noutput_items;
    }

	void ccsds_tc_impl::on()
    {
    	stat = plop_sine;
    	sine_count = 0;
    	buf_reset();
    }
    
    void ccsds_tc_impl::off()
    {
    	stat = plop_off;
    }
	
	void ccsds_tc_impl::buf_reset()
	{
		buflen = bufpos = 0;
	}
	
	void ccsds_tc_impl::buf_push(uint8_t x)
	{
		buf[buflen++] = x;
		if(buflen >= sizeof(buf))
			std::cerr << "ccsds_tc buf overflow!" << std::endl;
	}
	
	bool ccsds_tc_impl::start()
	{
		on();
		return true;
	}
	
	void ccsds_tc_impl::make_cltus()
	{
		if(q.empty()) return;
		std::vector<uint8_t> vp = q.front();
		q.pop();
		if(vp.empty()) return;
		uint8_t* packet = vp.data();
		//std::cout << "get a message " << size << std::endl;
		
		//data randomize
      	if(randomizer) {
    		for(int i = 0; i < up_ip_bytes; i++)
    			packet[i] ^= pn9[i % 255];
    	}    
    	
      	//clear buffer
      	buf_reset();
      	  			
      	//start sequence
		buf_push(0xeb);
		buf_push(0x90);
		
		for(int j = 0; j < up_ip_bytes; j += 7)
		{
			//write data
			for(int i = 0; i < 7; i++)
				buf_push(packet[i + j]);
			
			//calculate parity
			uint8_t parity = 0;
			for(int i = 0; i < 56; i++)
			{
				//scan from byte 0 to n-1,
				//bit from MSB to LSB
				if(packet[j + i/8] & (0x80 >> (i%8)))
					parity ^= bch64_p[i];
			}
			parity = (~parity) & 0xfe;//clear filler bit
		
			//write parity
			buf_push(parity);
		}
      	
      	//tail sequence
      	buf_push(0xc5);
      	buf_push(0xc5);
      	buf_push(0xc5);
      	buf_push(0xc5);
      	buf_push(0xc5);
      	buf_push(0xc5);
      	buf_push(0xc5);
      	buf_push(0x79);
	}

  } /* namespace kcsa */
} /* namespace gr */

