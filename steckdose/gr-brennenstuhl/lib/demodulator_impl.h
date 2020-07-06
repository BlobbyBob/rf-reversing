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

#ifndef INCLUDED_BRENNENSTUHL_DEMODULATOR_IMPL_H
#define INCLUDED_BRENNENSTUHL_DEMODULATOR_IMPL_H

#include <brennenstuhl/demodulator.h>

namespace gr {
    namespace brennenstuhl {

        class demodulator_impl : public demodulator {
        private:
            bool d_idle = true;
            bool d_sync = false;
            unsigned d_symb_len = 1;
            unsigned d_symb_pos = 0;
            unsigned d_symb_zeros = 0, d_symb_ones = 0;
            unsigned char d_last_sample = 0;

            bool d_debug;
            int d_max_symbol_len;
            int d_min_symbol_len;
            int d_fixed_symbol_len;

            std::vector<char> d_packet;
            const pmt::pmt_t d_out;

        public:
            demodulator_impl(bool debug, int max_symbol_len, int min_symbol_len, int fixed_symbol_len);

            ~demodulator_impl();

            int general_work(int noutput_items,
                             gr_vector_int &ninput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items);

        };

    } // namespace brennenstuhl
} // namespace gr

#endif /* INCLUDED_BRENNENSTUHL_DEMODULATOR_IMPL_H */

