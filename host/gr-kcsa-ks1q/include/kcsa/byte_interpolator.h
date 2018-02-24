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


#ifndef INCLUDED_KCSA_BYTE_INTERPOLATOR_H
#define INCLUDED_KCSA_BYTE_INTERPOLATOR_H

#include <kcsa/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace kcsa {

    /*!
     * \brief <+description of block+>
     * \ingroup kcsa
     *
     */
    class KCSA_API byte_interpolator : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<byte_interpolator> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of kcsa::byte_interpolator.
       *
       * To avoid accidental use of raw pointers, kcsa::byte_interpolator's
       * constructor is in a private implementation
       * class. kcsa::byte_interpolator::make is the public interface for
       * creating new instances.
       */
      static sptr make(int interp);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_BYTE_INTERPOLATOR_H */

