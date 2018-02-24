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
#include "halfduplex_pdu_to_stream_impl.h"

namespace gr {
namespace kcsa {

halfduplex_pdu_to_stream::sptr
halfduplex_pdu_to_stream::make(uint8_t acqsamples, int acqcount, bool flenmode, uint32_t syncword, int syncbits)
{
    return gnuradio::get_initial_sptr
            (new halfduplex_pdu_to_stream_impl(acqsamples, acqcount, flenmode, syncword, syncbits));
}

/*
     * The private constructor
     */
halfduplex_pdu_to_stream_impl::halfduplex_pdu_to_stream_impl(uint8_t acqsamples,
                                                             int acqcount, bool flenmode, uint32_t syncword, int syncbits)
    : gr::sync_decimator("halfduplex_pdu_to_stream",
                         gr::io_signature::make(0, 0, 0),
                         gr::io_signature::make(1, 1, sizeof(uint8_t)), 1),
      d_acqsamples(acqsamples),
      d_acqcount(acqcount),
      d_flenmode(flenmode),
      d_syncword(syncword),
      d_syncbits(syncbits),
      d_state(0)
{
    message_port_register_in(pmt::mp("pdu"));
    set_msg_handler(pmt::mp("pdu"), boost::bind(&halfduplex_pdu_to_stream_impl::pdu_callback, this ,_1));
}

/*
     * Our virtual destructor.
     */
halfduplex_pdu_to_stream_impl::~halfduplex_pdu_to_stream_impl()
{
}
void halfduplex_pdu_to_stream_impl::pdu_callback(pmt::pmt_t msg)
{
    size_t msg_len;
    size_t i = 0;
    uint32_t mask = (0x1 << d_syncbits) - 1;
    uint32_t syncword = 0x55555555;

    syncword &= ~mask;
    syncword |= (d_syncword & mask);

    //pmt::pmt_t meta = pmt::car(msg);
    pmt::pmt_t data = pmt::cdr(msg);

    //get a message, push into queue
    const uint8_t* bytes_in = pmt::u8vector_elements(data, msg_len);

    syncword = BigLittleSwap32(syncword);

    if(!d_flenmode) //False:Variable Frame Length;   True:Fixed Frame Length,don't fill
    {
        std::vector<uint8_t> vmsg(sizeof(uint32_t) + sizeof(uint16_t) + msg_len);
        //insert syncword
        memcpy(vmsg.data(), &syncword, sizeof(uint32_t));

        //insert frame length
        uint16_t flen = msg_len;
        //flen = BigLittleSwap16(flen);
        memcpy(vmsg.data() + sizeof(uint32_t), &flen, sizeof(uint16_t));

        //insert frame  data
        memcpy(vmsg.data() + sizeof(uint32_t) + sizeof(uint16_t), bytes_in, msg_len);

        gr::thread::scoped_lock guard(d_mutex);
        //insert frame  data queue
        q.push(vmsg);
    }
    else
    {
        std::vector<uint8_t> vmsg(sizeof(uint32_t) + msg_len);
        //insert syncword
        memcpy(vmsg.data(), &syncword, sizeof(uint32_t));

        //insert frame  data
        memcpy(vmsg.data() + sizeof(uint32_t), bytes_in, msg_len);

        gr::thread::scoped_lock guard(d_mutex);
        //insert frame  data queue
        q.push(vmsg);
    }
}

int
halfduplex_pdu_to_stream_impl::work(int noutput_items,
                                    gr_vector_const_void_star &input_items,
                                    gr_vector_void_star &output_items)
{
    uint8_t* out = (uint8_t*)output_items[0];
    int i = 0;

    if (q.empty())
        return 0;

    while(i < noutput_items)
    {
        switch (d_state)
        {
        case 0://send tx sob
        {
            add_item_tag(0, nitems_written(0) + 0,
                         pmt::string_to_symbol("tx_sob"),
                         pmt::from_bool(true)
                         );
            //insert acqsamples
            for(int j = 0; (j < d_acqcount) && (j < noutput_items); j++)
            {
                out[i++] = d_acqsamples;
            }
            d_state = 1;
        }
        case 1://insert pdu data
        {
            gr::thread::scoped_lock guard(d_mutex);
            std::vector<uint8_t> vdata = q.front();

            if(i + vdata.size () < noutput_items)
            {
                q.pop();
                //insert data
                for(int j = 0; j < vdata.size (); j++)
                {
                    out[i++] = vdata[j];
                }
                if(q.empty())
                {
                    d_state = 2;
                }
            }
            else
            {
                return i;
            }
            break;
        }
        case 2://send tx eob
        {
            if (i + 6 < noutput_items)
            {
                //insert tailer with several 0x55 for telling the hdardware don't shutdown early
                for(int j = 0; j < 6; j++)
                {
                    out[i++] = d_acqsamples;
                }

                //send tx eob
                add_item_tag(0, nitems_written(0) + i,
                             pmt::string_to_symbol("tx_eob"),
                             pmt::from_bool(false));
                d_state = 0;
            }
            return i;
        }
        default:
            d_state = 0;
            std::cerr << "invaild state default" << std::endl;
            return i;
        }
    }

    return i;
}

} /* namespace kcsa */
} /* namespace gr */

