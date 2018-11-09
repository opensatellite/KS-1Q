/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_KCSA_PSK_RX_H
#define INCLUDED_KCSA_PSK_RX_H

#include <kcsa/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace kcsa {

    /*!
     * \brief <+description of block+>
     * \ingroup kcsa
     *
     */
    class KCSA_API psk_rx : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<psk_rx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of kcsa::psk_rx.
       *
       * To avoid accidental use of raw pointers, kcsa::psk_rx's
       * constructor is in a private implementation
       * class. kcsa::psk_rx::make is the public interface for
       * creating new instances.
       */
      static sptr make(int _mod, int sps, float sym_rate, float sym_rate_dev, float sym_bw, float carr_rate, float carr_rate_dev, float carr_bw);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_PSK_RX_H */

