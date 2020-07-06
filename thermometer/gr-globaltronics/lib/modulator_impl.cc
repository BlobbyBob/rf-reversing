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
    namespace globaltronics {

        modulator::sptr
        modulator::make(int sample_rate, int repititions) {
            return gnuradio::get_initial_sptr
                    (new modulator_impl(sample_rate, repititions));
        }


        /*
         * The private constructor
         */
        modulator_impl::modulator_impl(int sample_rate, int repititions)
                : gr::sync_block("modulator",
                                 gr::io_signature::make(0, 0, 0),
                                 gr::io_signature::make(1, 1, sizeof(uint8_t))),
                  d_sample_rate(sample_rate),
                  d_repititions(repititions),
                  d_symlen_short(sample_rate / 496),
                  d_symlen_long(sample_rate / 249),
                  d_symlen_intermediate(sample_rate / 111),
                  d_symlen_active(sample_rate / 1942),
                  d_ifs(sample_rate * 7 / 2),
                  d_ifs_left(sample_rate),
                  d_curr_sample(0),
                  d_curr_sample_left(0),
                  d_packet({'1', '0', '0', '0', '0', '1', '0', '0', '0',
                            '1', '1', '1', '0', '0', '1', '1', '1',
                            '0', '1', '1', '1', '0', '0', '0', '0',
                            '1', '0', '1', '0', '0', '1', '0', '0'}),
                  d_symbols_left() {


        }

        /*
         * Our virtual destructor.
         */
        modulator_impl::~modulator_impl() {
        }

        int modulator_impl::work(int noutput_items,
                                 gr_vector_const_void_star &input_items,
                                 gr_vector_void_star &output_items) {
            auto *out = (uint8_t *) output_items[0];

            for (int i = 0; i < noutput_items; i++) {
                uint8_t sample = 0;

                if (d_ifs_left > 0) {
                    // Wait until we send frame
                    sample = 0;
                    if (--d_ifs_left == 0) {
                        // Prepare new frame to send
                        this->prepare_frame();
                        d_curr_sample = 1;
                        d_curr_sample_left = d_symlen_active;
                    }
                } else {
                    if (d_curr_sample_left == 0) {
                        if (d_curr_sample == 0) {
                            // Last sample was 0, so we are sending a 1 now
                            d_curr_sample = 1;
                            d_curr_sample_left = d_symlen_active;
                        } else {
                            // Last sample was 1, so send a 0 with a new symbol length
                            if (!d_symbols_left.empty()) {
                                switch (d_symbols_left.front()) {
                                    case '0':
                                        d_curr_sample_left = d_symlen_long;
                                        break;
                                    case '1':
                                        d_curr_sample_left = d_symlen_short;
                                        break;
                                    case '-':
                                        d_curr_sample_left = d_symlen_intermediate;
                                        break;
                                    default:
                                        std::cerr << "[ERR]  Unknown symbol: " << d_symbols_left.front() << std::endl;
                                }
                                d_curr_sample = 0;
                                d_symbols_left.pop();
                            } else {
                                // No more symbols to send, end frame
                                d_ifs_left = d_ifs;
                                sample = 0;
                                std::cerr << "[INFO] Sent frame" << std::endl;
                            }
                        }
                    }

                    // Send sample
                    if (d_ifs_left <= 0) {
                        sample = d_curr_sample;
                        d_curr_sample_left--;
                    }
                }

                out[i] = sample;
            }

            return noutput_items;
        }

        // Build our symbol queue
        void modulator_impl::prepare_frame() {
            while (!d_symbols_left.empty()) {
                d_symbols_left.pop();
            }

            for (int i = 0; i < d_repititions; i++) {
                if (i > 0) {
                    d_symbols_left.push('-');
                    d_symbols_left.push('1');
                    d_symbols_left.push('-');
                }
                for (auto const symbol: d_packet) {
                    d_symbols_left.push(symbol);
                }
            }
            d_symbols_left.push('-');
        }

    } /* namespace globaltronics */
} /* namespace gr */

