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

#ifndef INCLUDED_GLOBALTRONICS_MODULATOR_H
#define INCLUDED_GLOBALTRONICS_MODULATOR_H

#include <globaltronics/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
    namespace globaltronics {

        /*!
         * \brief <+description of block+>
         * \ingroup globaltronics
         *
         */
        class GLOBALTRONICS_API modulator

        : virtual public gr::sync_block {
        public:
        typedef boost::shared_ptr <modulator> sptr;

        /*!
         * \brief Return a shared_ptr to a new instance of globaltronics::modulator.
         *
         * To avoid accidental use of raw pointers, globaltronics::modulator's
         * constructor is in a private implementation
         * class. globaltronics::modulator::make is the public interface for
         * creating new instances.
         */
        static sptr make(int sample_rate, int repititions);
    };

} // namespace globaltronics
} // namespace gr

#endif /* INCLUDED_GLOBALTRONICS_MODULATOR_H */

