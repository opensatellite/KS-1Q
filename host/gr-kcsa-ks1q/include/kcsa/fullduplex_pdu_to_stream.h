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


#ifndef INCLUDED_KCSA_FULLDUPLEX_PDU_TO_STREAM_H
#define INCLUDED_KCSA_FULLDUPLEX_PDU_TO_STREAM_H

#include <kcsa/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace kcsa {

    /*!
     * \brief <+description of block+>
     * \ingroup kcsa
     *
     */
    class KCSA_API fullduplex_pdu_to_stream : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<fullduplex_pdu_to_stream> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of kcsa::fullduplex_pdu_to_stream.
       *
       * To avoid accidental use of raw pointers, kcsa::fullduplex_pdu_to_stream's
       * constructor is in a private implementation
       * class. kcsa::fullduplex_pdu_to_stream::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint8_t acqsamples, bool flenmode, uint32_t syncword, int syncbits);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_FULLDUPLEX_PDU_TO_STREAM_H */

