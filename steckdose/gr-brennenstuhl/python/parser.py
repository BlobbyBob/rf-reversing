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


class parser(gr.basic_block):
    """
    docstring for block brennenstuhl_parser
    """

    def __init__(self, debug=False):
        gr.basic_block.__init__(self,
                                name="brennenstuhl_parser",
                                in_sig=None,
                                out_sig=None)

        self.__debug = debug

        self.message_port_register_in(pmt.intern("packet"))
        self.set_msg_handler(pmt.intern("packet"), self.parse_packet)

    def parse_packet(self, packet):

        if not pmt.is_symbol(packet):
            if self.__debug:
                print("PMT is no symbol (string)")
            return

        packet = pmt.to_python(packet)

        if len(packet) != 24:
            if self.__debug:
                print("Message length invalid:", len(packet))
            return

        if '11' in packet:
            if self.__debug:
                print("Discarding packet, because of repetitive 1's")
            return

        if packet[20] == '1' and packet[22] == '1':
            if self.__debug:
                print("Discarding packet, because on and off is both set")
            return

        info = "DIP "
        if packet[0] == '0':
            info += "1"
        if packet[2] == '0':
            info += "2"
        if packet[4] == '0':
            info += "3"
        if packet[6] == '0':
            info += "4"
        if packet[8] == '0':
            info += "5"

        info += " Key "
        if packet[10] == '0':
            info += "A"
        if packet[12] == '0':
            info += "B"
        if packet[14] == '0':
            info += "C"
        if packet[16] == '0':
            info += "D"
        if packet[18] == '0':
            info += "E"

        if packet[20] == '1':
            info += " off"
        if packet[22] == '1':
            info += " on"

        print(info)
