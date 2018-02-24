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
#include "framesync_impl.h"
#include <sstream>
#include <iomanip>

//link with -mpopcnt
static inline int nsetbits(uint64_t x)
{
    if(x == 0) return 0;
    return __builtin_popcountll(x);
}

static inline int nsetbits(__uint128_t x)
{
    if(x == 0) return 0;
    uint64_t *p = (uint64_t*)&x;
    return nsetbits(p[0]) + nsetbits(p[1]);
}

static inline int nsetbits(boost::multiprecision::uint256_t x)
{
    if(x == 0) return 0;
    std::size_t size = x.backend().size();
    limb_type* p = x.backend().limbs();
    int n = 0;
    for(int i = 0; i < size; i++)
        n += nsetbits(*p++);
    return n;
}

namespace gr {
  namespace kcsa {

    framesync::sptr
    framesync::make(int mode, int nrz, std::string syncword,
        int syncbits, int maxerrs, int framebits)
    {
      return gnuradio::get_initial_sptr
        (new framesync_impl(mode, nrz, syncword, syncbits, maxerrs, framebits));
    }

    boost::multiprecision::uint256_t framesync_impl::syncmask_from_nbits(int n) {
        boost::multiprecision::uint256_t ret = 0;
        for(int i = 0; i < n; i++) {
            ret |= boost::multiprecision::uint256_t(1) << i;
        }
        return ret;
    }
    std::string framesync_impl::to_hex_string(uint64_t x) {
        std::stringstream stream;
        stream << std::hex << x;
        return stream.str();
    }
    std::string framesync_impl::to_hex_string(__uint128_t x) {
        std::stringstream stream;
        stream << std::hex << ((uint64_t)(x >> 64)) << (uint64_t)x;
        return stream.str();
    }
    std::string framesync_impl::to_hex_string(boost::multiprecision::uint256_t x) {
        std::stringstream stream;
        stream << std::hex << x;
        return stream.str();
    }
    std::string framesync_impl::shift_type_string(struct shift_type_s x, enum shift_len_e len) {
        switch(len) {
            case u64:  return to_hex_string(x.u64 );
            case u128: return to_hex_string(x.u128);
            case u256: return to_hex_string(x.u256);
            default:   return std::string();
        }
    }
    std::string framesync_impl::syncword_string() {
        return shift_type_string(syncword[0], synclen);
    }
    std::string framesync_impl::syncmask_string() {
        return shift_type_string(syncmask, synclen);
    }

    inline uint64_t framesync_impl::inbits(char x) {
        return 0;
    }
    /*
     * The private constructor
     */
    framesync_impl::framesync_impl(int _mode, int _nrz, std::string _syncword,
        int _syncbits, int _maxerrs, int _framebits)
      : gr::sync_block("framesync",
              gr::io_signature::make(1, 2, sizeof(char)),
              gr::io_signature::make(0, 0, 0)),
              mode(_mode),
              nrz(_nrz),
              maxerrs(_maxerrs),
              framebits(_framebits)
    {
        outport = pmt::mp("out");
        message_port_register_out(outport);
        set_history(1);

        if(mode < mode_bit || mode > mode_8psk) {
            throw std::out_of_range("mode invaild");
            return;
        }

        if(nrz < nrz_l || nrz > nrz_s) {
            throw std::out_of_range("nrz invaild");
            return;
        }

        boost::multiprecision::uint256_t syncword_u256(_syncword);
        boost::multiprecision::uint256_t syncmask_u256(syncmask_from_nbits(_syncbits));

        if(_syncbits > 256 || _syncbits < 0) {
            throw std::out_of_range("syncbits");
        } /*else if(_syncbits > 128) {
            synclen = u256;
            syncword[0].u256 = syncword_u256;
            syncmask.u256 = syncmask_u256;
        } else if(_syncbits > 64) {
            synclen = u128;
            syncword[0].u128 = syncword_u256.convert_to<__uint128_t>();
            syncmask.u128 = syncmask_u256.convert_to<__uint128_t>();
        } */else {
            synclen = u64;
            syncword[0].u64 = syncword_u256.convert_to<uint64_t>();
            syncmask.u64 = syncmask_u256.convert_to<uint64_t>();
        }
        std::cout << "[Frame Sync] syncword: " << syncword_string() << " mask: " << syncmask_string() << std::endl;


    }

    /*
     * Our virtual destructor.
     */
    framesync_impl::~framesync_impl()
    {
    }

    bool framesync_impl::lock()
    {
        return d_lock_state;
    }

    int
    framesync_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];

      // Do <+signal processing+>
      switch(synclen) {
          case u64: {
            for(int i = 0; i < noutput_items; i++) {
                shiftreg.u64 <<= 1;
                shiftreg.u64 |= *in++ ? 1 : 0;
                readbit(shiftreg.u64);
                uint64_t test = (shiftreg.u64 ^ syncword[0]) & syncmask;
                if(nsetbits(test) <= maxerrs) {

                }
            }
          } break;
          /*
          case u128: {
            for(int i = 0; i < noutput_items; i++) {
                shiftreg.u128 <<= 1;
                shiftreg.u128 |= *in++ ? 1 : 0;
            }
          } break;
          case u256: {
            for(int i = 0; i < noutput_items; i++) {
                shiftreg.u256 <<= 1;
                shiftreg.u256 |= *in++ ? 1 : 0;
            }
          } break;
*/
          default:
          throw std::out_of_range("synclen");
          return;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */
