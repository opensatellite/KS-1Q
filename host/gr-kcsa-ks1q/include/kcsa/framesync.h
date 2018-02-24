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


#ifndef INCLUDED_KCSA_FRAMESYNC_H
#define INCLUDED_KCSA_FRAMESYNC_H

#include <kcsa/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace kcsa {

    /*!
     * \brief <+description of block+>
     * \ingroup kcsa
     *
     */
    class KCSA_API framesync : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<framesync> sptr;

       static const int mode_bit = -1;
       static const int mode_bpsk = 0;
       static const int mode_qpsk = 1;
       static const int mode_oqpsk = 2;
       static const int mode_8psk = 3;

       static const int nrz_l = 0;
       static const int nrz_m = 1;
       static const int nrz_s = 2;

      /*!
       * \brief Return a shared_ptr to a new instance of kcsa::framesync.
       *
       * To avoid accidental use of raw pointers, kcsa::framesync's
       * constructor is in a private implementation
       * class. kcsa::framesync::make is the public interface for
       * creating new instances.
       */
      static sptr make(int mode, int nrz, std::string syncword, int syncbits, int maxerrs, int framebits);
      virtual bool lock() = 0;

    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_FRAMESYNC_H */
