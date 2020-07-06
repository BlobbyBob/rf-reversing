/* -*- c++ -*- */
/*
 * Copyright 2020 Ben Swierzy.
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

#ifndef INCLUDED_GLOBALTRONICS_DEMODULATOR_IMPL_H
#define INCLUDED_GLOBALTRONICS_DEMODULATOR_IMPL_H

#include <globaltronics/demodulator.h>

namespace gr {
    namespace globaltronics {

        class demodulator_impl : public demodulator {
        private:
            // Config
            pmt::pmt_t d_out;
            int d_fixed_symlen_short;
            int d_fixed_symlen_long;

            // Parser state
            bool d_idle;
            bool d_sync;
            bool d_sync_short;
            bool d_sync_long;

            // Constants for a packet
            int d_symlen_short;
            int d_symlen_long;

            // Dynamic Variables
            int d_zeros;
            int d_ones;
            uint8_t d_last_sample;
            std::vector<char> d_packet;

        public:
            demodulator_impl(int fixed_symlen_short, int fixed_symlen_long);

            ~demodulator_impl();

            // Where all the action really happens
            int general_work(int noutput_items,
                             gr_vector_int &ninput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items);
        };

    } // namespace globaltronics
} // namespace gr

#endif /* INCLUDED_GLOBALTRONICS_DEMODULATOR_IMPL_H */

