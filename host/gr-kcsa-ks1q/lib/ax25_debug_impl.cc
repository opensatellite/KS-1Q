/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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
#include "ax25_debug_impl.h"
#include <cstdio>

namespace gr {
  namespace kcsa {

    ax25_debug::sptr
    ax25_debug::make()
    {
      return gnuradio::get_initial_sptr
        (new ax25_debug_impl());
    }

    /*
     * The private constructor
     */
    ax25_debug_impl::ax25_debug_impl()
      : gr::sync_block("ax25_debug",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
    	d_pdu_port = pmt::mp("pdu");
    	message_port_register_in(d_pdu_port);
    	set_msg_handler(d_pdu_port, boost::bind(&ax25_debug_impl::pdu_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    ax25_debug_impl::~ax25_debug_impl()
    {
    }

	void ax25_debug_impl::pdu_callback(pmt::pmt_t msg)
	{
		pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t data(pmt::cdr(msg));
		
		size_t len;
		const uint8_t* bp = pmt::u8vector_elements(data, len);
			
		size_t total_len = len;
    unsigned char v1=1,cmd=0;
    unsigned char i,j;
    const uint8_t *hexptr = bp;
    
   if (bp[1] & 1) {
        /*
         * FlexNet Header Compression
         */
        v1 = 0;
        cmd = (bp[1] & 2) != 0;
        printf("AFSK1200: fm ? to ");
        i = (bp[2] >> 2) & 0x3f;
        if (i)
            printf("%c",i+0x20);
        i = ((bp[2] << 4) | ((bp[3] >> 4) & 0xf)) & 0x3f;
        if (i)
            printf("%c",i+0x20);
        i = ((bp[3] << 2) | ((bp[4] >> 6) & 3)) & 0x3f;
        if (i)
            printf("%c",i+0x20);
        i = bp[4] & 0x3f;
        if (i)
            printf("%c",i+0x20);
        i = (bp[5] >> 2) & 0x3f;
        if (i)
            printf("%c",i+0x20);
        i = ((bp[5] << 4) | ((bp[6] >> 4) & 0xf)) & 0x3f;
        if (i)
            printf("%c",i+0x20);
        printf("-%u QSO Nr %u", bp[6] & 0xf, (bp[0] << 6) | (bp[1] >> 2));
        bp += 7;
        len -= 7;
    } else {         /*
               * normal header
               */
        if (len < 15)
            return;
        if ((bp[6] & 0x80) != (bp[13] & 0x80)) {
            v1 = 0;
            cmd = (bp[6] & 0x80);
        }
        printf("AFSK1200: fm ");
        for(i = 7; i < 13; i++)
            if ((bp[i] &0xfe) != 0x40)
                printf("%c",bp[i] >> 1);
        //printf("-%u to ",(bp[13] >> 1) & 0xf);
        printf("-%c to ",(bp[13] >> 1) & 0x7f);
        for(i = 0; i < 6; i++)
            if ((bp[i] &0xfe) != 0x40)
                printf("%c",bp[i] >> 1);
        //printf("-%u",(bp[6] >> 1) & 0xf);
        printf("-%c",(bp[6] >> 1) & 0x7f);
        bp += 14;
        len -= 14;
        if ((!(bp[-1] & 1)) && (len >= 7))
            printf(" via ");
        while ((!(bp[-1] & 1)) && (len >= 7)) {
            for(i = 0; i < 6; i++)
                if ((bp[i] &0xfe) != 0x40)
                    printf("%c",bp[i] >> 1);
            printf("-%u",(bp[6] >> 1) & 0xf);
            bp += 7;
            len -= 7;
            if ((!(bp[-1] & 1)) && (len >= 7))
                printf(",");
        }
    }
    if(!len)
        return;
    i = *bp++;
    len--;
    j = v1 ? ((i & 0x10) ? '!' : ' ') :
             ((i & 0x10) ? (cmd ? '+' : '-') : (cmd ? '^' : 'v'));
    if (!(i & 1)) {
        /*
                               * Info frame
                               */
        printf(" I%u%u%c",(i >> 5) & 7,(i >> 1) & 7,j);
    } else if (i & 2) {
        /*
                           * U frame
                           */
        switch (i & (~0x10)) {
        case 0x03:
            printf(" UI%c",j);
            break;
        case 0x2f:
            printf(" SABM%c",j);
            break;
        case 0x43:
            printf(" DISC%c",j);
            break;
        case 0x0f:
            printf(" DM%c",j);
            break;
        case 0x63:
            printf(" UA%c",j);
            break;
        case 0x87:
            printf(" FRMR%c",j);
            break;
        default:
            printf(" unknown U (0x%x)%c",i & (~0x10),j);
            break;
        }
    } else {
        /*
                           * supervisory
                           */
        switch (i & 0xf) {
        case 0x1:
            printf(" RR%u%c",(i >> 5) & 7,j);
            break;
        case 0x5:
            printf(" RNR%u%c",(i >> 5) & 7,j);
            break;
        case 0x9:
            printf(" REJ%u%c",(i >> 5) & 7,j);
            break;
        default:
            printf(" unknown S (0x%x)%u%c", i & 0xf,
                       (i >> 5) & 7, j);
            break;
        }
    }
    if (!len) {
        printf(" len = %lu\n", total_len);
        return;
    }
    printf(" pid=%02X len = %lu\n", *bp++, total_len);
    
    bool print_hex = false;
    len--;
    j = 0;
//    const uint8_t *hexptr = bp;
    size_t hexlen = len;
    while (len) {
        i = *bp++;
        if ((i >= 32) && (i < 128))
            printf("%c",i);
        else if (i == 13) {
            if (j)
                printf("\n");
            j = 0;
        } else {
            printf(".");
            print_hex = true;
        }
        if (i >= 32)
            j = 1;
        len--;
    }
    if (j) printf("\n");
    
    	if(print_hex) {
    		size_t count = 0;
    		printf("hex dump = \n");
    		while(hexlen--) {
    			printf("%02x ", *hexptr++);
    			count++;
    			if(count && (count % 16 == 0)) {
    				printf("\n");
    			}
    		}
    		if(!(count && (count % 16 == 0))) printf("\n");
    		printf("***********************************\n");
    	}
	}

    int
    ax25_debug_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */

