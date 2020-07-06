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

#ifndef INCLUDED_GLOBALTRONICS_MODULATOR_IMPL_H
#define INCLUDED_GLOBALTRONICS_MODULATOR_IMPL_H

#include <queue>
#include <globaltronics/modulator.h>

namespace gr {
    namespace globaltronics {

        class modulator_impl : public modulator {
        private:
            // Config
            int d_sample_rate;
            int d_symlen_short;
            int d_symlen_long;
            int d_symlen_intermediate;
            int d_symlen_active;
            int d_repititions;
            int d_ifs;
            std::vector<char> d_packet;

            // Dynamic
            int d_ifs_left;
            uint8_t d_curr_sample;
            int d_curr_sample_left;
            std::queue<char> d_symbols_left;

        public:
            modulator_impl(int sample_rate, int repititions);

            ~modulator_impl();

            // Where all the action really happens
            int work(
                    int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items
            );

            void prepare_frame();
        };

    } // namespace globaltronics
} // namespace gr

#endif /* INCLUDED_GLOBALTRONICS_MODULATOR_IMPL_H */

