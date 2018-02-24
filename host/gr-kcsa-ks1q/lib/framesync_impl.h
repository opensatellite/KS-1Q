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

#ifndef INCLUDED_KCSA_FRAMESYNC_IMPL_H
#define INCLUDED_KCSA_FRAMESYNC_IMPL_H

#include <kcsa/framesync.h>
#include <boost/multiprecision/cpp_int.hpp>

//class boost::multiprecision::uint256_t;
namespace gr {
  namespace kcsa {

    class framesync_impl : public framesync
    {
     private:
      int mode;//-1 = bit, 0 = bpsk, 1 = qpsk, 2 = oqpsk, 3 = 8psk
      int nrz;//0 = L, 1 = M, 2 = S
      //std::vector<uint8_t> syncword;
      //int syncbits;
      int maxerrs;
      int framebits;

      static const int syncword_n = 8;
      struct shift_type_s {
          uint64_t u64;
          __uint128_t u128;
          boost::multiprecision::uint256_t u256;
      } shiftreg, syncmask, syncword[syncword_n];
      enum shift_len_e { u64, u128, u256 } synclen;
      std::vector<uint8_t> buffer;
      enum state_e {state_search, state_syncword, state_payload};
      int syncmode;//0-4, see framesync_modes.txt
      int graycode;//0-7, see framesync_modes.txt
      bool rx_payload;

      pmt::pmt_t outport;

      boost::multiprecision::uint256_t syncmask_from_nbits(int n);
      std::string to_hex_string(uint64_t x);
      std::string to_hex_string(__uint128_t x);
      std::string to_hex_string(boost::multiprecision::uint256_t x);
      std::string shift_type_string(struct shift_type_s x, enum shift_len_e len);
      std::string syncword_string();
      std::string syncmask_string();

      inline uint64_t inbits(char x);

     public:
      framesync_impl(int _mode, int _nrz, std::string _syncword,
          int _syncbits, int _maxerrs, int _framebits);
      ~framesync_impl();

      bool lock();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_FRAMESYNC_IMPL_H */
