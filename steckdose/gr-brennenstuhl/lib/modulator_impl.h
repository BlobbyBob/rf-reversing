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

#ifndef INCLUDED_BRENNENSTUHL_MODULATOR_IMPL_H
#define INCLUDED_BRENNENSTUHL_MODULATOR_IMPL_H

#include <brennenstuhl/modulator.h>
#include <queue>

namespace gr {
  namespace brennenstuhl {

    class modulator_impl : public modulator
    {
     private:
        bool d_done = false;
        int d_symbol_length;
        int d_ifs_length;

        std::queue<std::queue<std::vector<uint8_t>*>*> d_frame_sequences;
        std::vector<uint8_t> *d_curr_frame;
        int d_ifs_left;
        int d_symbol_left;
        int d_curr_symbol_index;

        const int d_one_high_length;
        const int d_zero_high_length;

     public:
      modulator_impl(int symbol_length, int ifs_length);
      ~modulator_impl();

        // Where all the action really happens
        int work(
                int noutput_items,
                gr_vector_const_void_star &input_items,
                gr_vector_void_star &output_items
        );

    };

  } // namespace brennenstuhl
} // namespace gr

#endif /* INCLUDED_BRENNENSTUHL_MODULATOR_IMPL_H */

