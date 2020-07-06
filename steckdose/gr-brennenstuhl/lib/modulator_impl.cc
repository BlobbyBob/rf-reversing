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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "modulator_impl.h"

namespace gr {
    namespace brennenstuhl {

        modulator::sptr
        modulator::make(int symbol_length, int ifs_length) {
            return gnuradio::get_initial_sptr
                    (new modulator_impl(symbol_length, ifs_length));
        }


        /*
         * The private constructor
         */
        modulator_impl::modulator_impl(int symbol_length, int ifs_length)
                : gr::sync_block("modulator",
                                 gr::io_signature::make(0, 0, 0),
                                 gr::io_signature::make(1, 1, sizeof(uint8_t))),
                  d_ifs_length(ifs_length),
                  d_ifs_left(ifs_length * 300),
                  d_symbol_length(symbol_length),
                  d_zero_high_length(symbol_length / 4),
                  d_one_high_length(symbol_length * 3 / 4),
                  d_symbol_left(0),
                  d_curr_symbol_index(0),
                  d_frame_sequences(),
                  d_curr_frame(nullptr) {

            auto *frame_sequence = new std::queue<std::vector<uint8_t> *>();
            auto *frame = new std::vector<uint8_t>(
                    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0});
            frame_sequence->push(frame);
            frame = new std::vector<uint8_t>(
                    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0});
            frame_sequence->push(frame);
            frame = new std::vector<uint8_t>(
                    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0});
            frame_sequence->push(frame);
            frame = new std::vector<uint8_t>(
                    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0});
            frame_sequence->push(frame);

            d_frame_sequences.push(frame_sequence);

        }

        /*
         * Our virtual destructor.
         */
        modulator_impl::~modulator_impl() {
        }

        int modulator_impl::work(int noutput_items,
                                 gr_vector_const_void_star &input_items,
                                 gr_vector_void_star &output_items) {
            auto *mod = (uint8_t *) output_items[0];

            for (int i = 0; i < noutput_items; i++) {
                if (d_done) {
                    mod[i] = 0;
                    continue;
                }
                if (d_curr_frame == nullptr) {
                    if (d_ifs_left > 0) {
                        // Send nothing; wait for IFS to end
                        d_ifs_left--;
                        mod[i] = 0;
                    } else {
                        if (d_frame_sequences.empty()) {
                            std::cerr << "Done" << std::endl;
                            d_done = true;
                        }
                        // Load next frame if available
                        while (!d_frame_sequences.empty()) {
                            if (d_frame_sequences.front()->empty()) {
                                auto top = d_frame_sequences.front();
                                delete top;
                                d_frame_sequences.pop();
                                // Extra long time between frame sequences
                                d_ifs_left += 20 * d_ifs_length;
                            } else {
                                d_curr_frame = d_frame_sequences.front()->front();
                                d_frame_sequences.front()->pop();
                                // Init sending with the next sample
                                d_curr_symbol_index = 0;
                                d_symbol_left = d_symbol_length;
                                break;
                            }
                        }
                        mod[i] = 0;
                    }
                } else {
                    if (d_symbol_left == 0) {
                        // Jump to next symbol
                        d_curr_symbol_index++;
                        d_symbol_left = d_symbol_length;
                    }
                    if (d_curr_symbol_index < d_curr_frame->size()) {
                        // Send a 0 or 1 sample depending on our current bit
                        auto bit = d_curr_frame->at(d_curr_symbol_index);
                        if (bit == 0) {
                            mod[i] = (d_zero_high_length + d_symbol_left > d_symbol_length) ? 1 : 0;
                        } else {
                            mod[i] = (d_one_high_length + d_symbol_left > d_symbol_length) ? 1 : 0;
                        }
                        d_symbol_left--;
                    } else {
                        // We don't have any symbols to send. End frame
                        delete d_curr_frame;
                        d_curr_frame = nullptr;
                        d_ifs_left = d_ifs_length;
                        std::cerr << "Sent frame" << std::endl;
                    }
                }
            }

            // Tell runtime system how many output items we produced.
            return noutput_items;
        }

    } /* namespace brennenstuhl */
} /* namespace gr */

