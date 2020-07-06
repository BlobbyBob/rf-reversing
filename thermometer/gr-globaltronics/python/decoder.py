#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 Ben Swierzy.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#
import pmt
from gnuradio import gr


class decoder(gr.basic_block):
    """
    docstring for block globaltronics_decoder
    """

    def __init__(self):
        gr.basic_block.__init__(self,
                                name="globaltronics_decoder",
                                in_sig=None,
                                out_sig=None)

        self.message_port_register_in(pmt.intern("globaltronics_decode"))
        self.set_msg_handler(pmt.intern("globaltronics_decode"), self.decode_packet)

        self.__temp_offset = 81 - 22

    def decode_packet(self, packet):

        if not pmt.is_symbol(packet):
            print("[WARN] PMT is no symbol (string)")
            return

        packet = str(pmt.to_python(packet))
        print(packet)
        repititions = packet.split("-1-")

        if len(repititions) > 1 and len(repititions[1]) == 33 and not '?' in packet:
            payload = repititions[1]

            print(f"[INFO] Temperature: {self.getTemp(payload[9:17])}°C")

        else:
            print("[INFO] Invalid packet: " + packet)

    def getTemp(self, temp_encoded):

        temp_encoded = int(temp_encoded, 2)
        temp_with_offset = (255 & ~temp_encoded) >> 1
        temp = temp_with_offset - self.__temp_offset

        return temp
