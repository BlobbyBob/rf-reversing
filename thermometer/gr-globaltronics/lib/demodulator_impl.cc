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
#include "demodulator_impl.h"

namespace gr {
    namespace globaltronics {

        demodulator::sptr
        demodulator::make(int fixed_symlen_short, int fixed_symlen_long) {
            return gnuradio::get_initial_sptr
                    (new demodulator_impl(fixed_symlen_short, fixed_symlen_long));
        }


        /*
         * The private constructor
         */
        demodulator_impl::demodulator_impl(int fixed_symlen_short, int fixed_symlen_long)
                : gr::block("demodulator",
                            gr::io_signature::make(1, 1, 1),
                            gr::io_signature::make(0, 0, 0)),
                  d_idle(true),
                  d_sync(false),
                  d_sync_short(false),
                  d_sync_long(false),
                  d_zeros(0),
                  d_ones(0),
                  d_symlen_short(0),
                  d_symlen_long(0),
                  d_last_sample(0),
                  d_packet(),
                  d_out(pmt::mp("globaltronics_demod")),
                  d_fixed_symlen_short(fixed_symlen_short),
                  d_fixed_symlen_long(fixed_symlen_long) {

            message_port_register_out(d_out);

        }

        /*
         * Our virtual destructor.
         */
        demodulator_impl::~demodulator_impl() {
        }

        int demodulator_impl::general_work(int noutput_items,
                                           gr_vector_int &ninput_items,
                                           gr_vector_const_void_star &input_items,
                                           gr_vector_void_star &output_items) {
            const auto *in = (const uint8_t *) input_items[0];

            int i;
            for (i = 0; i < ninput_items[0]; i++) {
                auto sample = in[i];

                if (d_idle) {

                    if (!d_sync) {
                        // Wait for a signal
                        if (sample == 1) {
                            d_ones++;

                            if (d_ones > 20) {
                                // Start short sync
                                d_sync = true;
                                d_sync_short = true;
                                d_zeros = 0;
                            }
                        } else {
                            // Stay idle
                            d_ones = 0;
                        }
                    } else {
                        if (d_sync_short) {
                            if (sample == 0) {
                                d_zeros++;

                                if ((d_fixed_symlen_short > 0 && d_zeros >= 2 * d_fixed_symlen_short) ||
                                    (d_fixed_symlen_short < 0 && d_zeros > 20000)) {
                                    // Sync error, doesn't seem to be a packet for us
                                    d_sync = false;
                                    d_sync_short = false;
                                }

                            } else if (d_last_sample == 0) {
                                // End short sync
                                d_sync_short = false;
                                d_symlen_short = d_fixed_symlen_short < 0 ? d_zeros : d_fixed_symlen_short;
                                // Start long sync
                                d_zeros = 0;
                                d_sync_long = true;
                            } // else we are still in the block of ones, before the short symbol
                        } else if (d_sync_long) {
                            if (sample == 0) {
                                d_zeros++;
                            } else if (d_last_sample == 0) {
                                // End long sync
                                d_sync_long = false;
                                d_symlen_long = d_fixed_symlen_long < 0 ? d_zeros : d_fixed_symlen_long;
                                // Start receiving
                                d_zeros = 0;
                                d_sync = false;
                                d_idle = false;
                                d_packet.clear();
                            } // else we are still in the block of ones, before the long symbol
                        } else {
                            // Illegal state
                            // std::cerr << "[ERR]  Illegal state: d_sync, but not d_sync_{short,long}" << std::endl;
                        }
                    }

                } else {

                    // Receive
                    if (sample == 0) {

                        d_zeros++;
                        if (d_zeros > d_symlen_long * 3) {
                            // End of frame
                            std::string packet(d_packet.begin(), d_packet.end());
                            message_port_pub(d_out, pmt::mp(packet));
                            d_idle = true;
                        }

                    } else if (d_last_sample == 0) {

                        // End of symbol -> decode symbol depending on the length
                        if (d_zeros <= d_symlen_short / 2) {
                            // Symbol too short -> don't decode
                            // std::cerr << "[WARN] Symbol too short, ignoring end of symbol (d_zeros=" << d_zeros
                            //           << ", d_symlen_short=" << d_symlen_short << ")" << std::endl;
                        } else if (d_zeros <= d_symlen_short * 3 / 2) {
                            // Short symbol detected
                            d_packet.push_back('1');
                            d_zeros = 0;
                        } else if (d_zeros <= d_symlen_long * 2 / 3) {
                            // This case might never occur, we are between short and long and aren't sure what it is
                            // std::cerr << "[WARN] Can't decode symbol (d_zeros=" << d_zeros << ", d_symlen_short="
                            //           << d_symlen_short << ", d_symlen_long=" << d_symlen_long << ")" << std::endl;
                            d_packet.push_back('?');
                            d_zeros = 0;
                        } else if (d_zeros <= d_symlen_long * 3 / 2) {
                            // Long symbol detected
                            d_packet.push_back('0');
                            d_zeros = 0;
                        } else if (d_zeros <= d_symlen_long * 3) {
                            // Intermediate symbol detected
                            d_packet.push_back('-');
                            d_zeros = 0;
                        } else {
                            // Illegal state
                            // std::cerr << "[ERR]  Illegal state: d_zeros large, but frame not ended" << std::endl;
                        }

                    }
                }
                d_last_sample = sample;

            }

            consume_each(i);

            return noutput_items;
        }

    } /* namespace globaltronics */
} /* namespace gr */

