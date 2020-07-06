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
    namespace brennenstuhl {

        demodulator::sptr
        demodulator::make(bool debug, int max_symbol_len, int min_symbol_len, int fixed_symbol_len) {
            return gnuradio::get_initial_sptr
                    (new demodulator_impl(debug, max_symbol_len, min_symbol_len, fixed_symbol_len));
        }


        /*
         * The private constructor
         */
        demodulator_impl::demodulator_impl(bool debug, int max_symbol_len, int min_symbol_len, int fixed_symbol_len)
                : gr::block("demodulator",
                            gr::io_signature::make(1, 1, sizeof(unsigned char)),
                            gr::io_signature::make(0, 0, 0)),
                  d_debug(debug),
                  d_max_symbol_len(max_symbol_len),
                  d_min_symbol_len(min_symbol_len),
                  d_fixed_symbol_len(fixed_symbol_len),
                  d_packet(),
                  d_out(pmt::mp("brennenstuhl_demod")) {

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
            const auto *in = (const unsigned char *) input_items[0];

            int i;
            for (i = 0; i < ninput_items[0]; i++) {
                if (d_idle) {
                    if (!d_sync) {
                        if (in[i] == 1) {
                            if (d_debug) {
                                std::cerr << "Info: Found possible preamble." << std::endl;
                            }
                            d_sync = true;
                            d_symb_len = 1;
                        }
                    } else {
                        if (in[i] == 1 && d_last_sample == 0) {
                            if (d_symb_len >= d_min_symbol_len) {
                                // Use fixed symbol length if set
                                if (d_fixed_symbol_len > 0) d_symb_len = d_fixed_symbol_len;

                                // Transition to receiving state
                                if (d_debug) {
                                    std::cerr << "Info: Started Packet receiving. Symbol length: " << d_symb_len
                                              << std::endl;
                                }
                                d_sync = false;
                                d_idle = false;
                                d_symb_pos = 0;
                                d_symb_zeros = 0;
                                d_symb_ones = 0;
                                d_packet.clear();
                            } else {
                                d_sync = false;
                                if (d_debug) {
                                    std::cerr << "Info: Discarding preamble (symbol length too small)" << std::endl;
                                }
                            }
                        } else {
                            d_symb_len++;
                        }
                        if (d_symb_len > d_max_symbol_len) {
                            d_sync = false;
                            std::cerr << "Info: Discarding preamble (symbol length too large)" << std::endl;
                        }
                    }
                }

                if (!d_idle) {
                    switch (in[i]) {
                        case 0:
                            d_symb_zeros++;
                            break;
                        case 1:
                            d_symb_ones++;
                            break;
                        default:
                            if (d_debug) {
                                std::cerr << "Warning: received invalid sample with value " << +in[i] << std::endl;
                            }
                    }

                    d_symb_pos++;
                    if (d_symb_pos >= d_symb_len) {
                        // Stop the packet, if symbol is invalid, ie. doesn't contain enough ones
                        if (d_symb_ones <= d_symb_len / 10) {
                            std::string packet(d_packet.begin(), d_packet.end());

                            if (d_debug) {
                                std::cout << "Info: Received packet: " << packet << std::endl;
                            }
                            message_port_pub(d_out, pmt::mp(packet));

                            d_idle = true;
                        } else {
                            // We finished the symbol and add the bit (as ascii char) to our packet
                            d_packet.push_back((d_symb_zeros > d_symb_ones) ? '0' : '1');

                            // Reset current symbol
                            d_symb_pos = 0;
                            d_symb_zeros = 0;
                            d_symb_ones = 0;

                        }
                    }
                }
                d_last_sample = in[i];

            }

            // Tell runtime system how many input items we consumed on
            // each input stream.
            consume_each(i);

            // Tell runtime system how many output items we produced.
            return 0;
        }

    } /* namespace brennenstuhl */
} /* namespace gr */

