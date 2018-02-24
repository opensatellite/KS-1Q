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


#ifndef INCLUDED_KCSA_CCSDS_UDP_TC_H
#define INCLUDED_KCSA_CCSDS_UDP_TC_H

#include <kcsa/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace kcsa {

    /*!
     * \brief <+description of block+>
     * \ingroup kcsa
     *
     */
    class KCSA_API ccsds_udp_tc : virtual public sync_decimator
    {
     public:
      typedef boost::shared_ptr<ccsds_udp_tc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of kcsa::ccsds_udp_tc.
       *
       * To avoid accidental use of raw pointers, kcsa::ccsds_tc's
       * constructor is in a private implementation
       * class. kcsa::ccsds_tc::make is the public interface for
       * creating new instances.
       */
      static sptr make(char* addr, int port, int decim, bool randomizer, int sine_samples, int acq_samples);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_CCSDS_TC_H */

