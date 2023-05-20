/* Lzip - LZMA lossless data compressor
     Copyright (C) 2008-2022 Antonio Diaz Diaz.

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _FILE_OFFSET_BITS 64

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <stdint.h>
#include <io.h>

#include "lzip.h"
#include "decoder.h"

bool Range_decoder::read_block()
{
    if (!at_stream_end)
    {
        memcpy(buffer, inputStream + stream_pos,
               (buffer_size < inputStreamLength - stream_pos) ? buffer_size : inputStreamLength - stream_pos);
        stream_pos += (buffer_size < inputStreamLength - stream_pos) ? buffer_size : inputStreamLength - stream_pos;
        at_stream_end = (stream_pos < buffer_size);
        partial_member_pos += pos;
        pos = 0;
    }
    return pos < stream_pos;
}

void LZ_decoder::flush_data()
{
    if (pos > stream_pos)
    {
        const int size = pos - stream_pos;
        // crc32.update_buf( crc_, buffer + stream_pos, size );
        //    if( outfd >= 0 && writeblock( outfd, buffer + stream_pos, size ) != size )
        //      throw Error( "Write error" );
        memcpy(outputStream + *outputStreamLength, buffer + stream_pos, size);
        *outputStreamLength += size;
        if (pos >= dictionary_size)
        {
            partial_data_pos += pos;
            pos = 0;
            pos_wrapped = true;
        }
        stream_pos = pos;
    }
}

/* Return value: 0 = OK, 1 = decoder error, 2 = unexpected EOF,
                                 3 = trailer error, 4 = unknown marker found. */
int LZ_decoder::decode_member()
{
    Bit_model bm_literal[1 << literal_context_bits][0x300];
    Bit_model bm_match[State::states][pos_states];
    Bit_model bm_rep[State::states];
    Bit_model bm_rep0[State::states];
    Bit_model bm_rep1[State::states];
    Bit_model bm_rep2[State::states];
    Bit_model bm_len[State::states][pos_states];
    Bit_model bm_dis_slot[len_states][1 << dis_slot_bits];
    Bit_model bm_dis[modeled_distances - end_dis_model + 1];
    Bit_model bm_align[dis_align_size];
    Len_model match_len_model;
    Len_model rep_len_model;
    unsigned rep0 = 0; // rep[0-3] latest four distances
    unsigned rep1 = 0; // used for efficient coding of
    unsigned rep2 = 0; // repeated distances
    unsigned rep3 = 0;
    State state;

    rdec.load();
    while (!rdec.finished())
    {
        const int pos_state = data_position() & pos_state_mask;
        if (rdec.decode_bit(bm_match[state()][pos_state]) == 0) // 1st bit
        {
            // literal byte
            Bit_model *const bm = bm_literal[get_lit_state(peek_prev())];
            if (state.is_char_set_char())
                put_byte(rdec.decode_tree8(bm));
            else
                put_byte(rdec.decode_matched(bm, peek(rep0)));
            continue;
        }
        // match or repeated match
        int len;
        if (rdec.decode_bit(bm_rep[state()]) != 0) // 2nd bit
        {
            if (rdec.decode_bit(bm_rep0[state()]) == 0) // 3rd bit
            {
                if (rdec.decode_bit(bm_len[state()][pos_state]) == 0) // 4th bit
                {
                    state.set_short_rep();
                    put_byte(peek(rep0));
                    continue;
                }
            }
            else
            {
                unsigned distance;
                if (rdec.decode_bit(bm_rep1[state()]) == 0) // 4th bit
                    distance = rep1;
                else
                {
                    if (rdec.decode_bit(bm_rep2[state()]) == 0) // 5th bit
                        distance = rep2;
                    else
                    {
                        distance = rep3;
                        rep3 = rep2;
                    }
                    rep2 = rep1;
                }
                rep1 = rep0;
                rep0 = distance;
            }
            state.set_rep();
            len = rdec.decode_len(rep_len_model, pos_state);
        }
        else // match
        {
            len = rdec.decode_len(match_len_model, pos_state);
            unsigned distance = rdec.decode_tree6(bm_dis_slot[get_len_state(len)]);
            if (distance >= start_dis_model)
            {
                const unsigned dis_slot = distance;
                const int direct_bits = (dis_slot >> 1) - 1;
                distance = (2 | (dis_slot & 1)) << direct_bits;
                if (dis_slot < end_dis_model)
                    distance += rdec.decode_tree_reversed(
                        bm_dis + (distance - dis_slot), direct_bits);
                else
                {
                    distance +=
                        rdec.decode(direct_bits - dis_align_bits) << dis_align_bits;
                    distance += rdec.decode_tree_reversed4(bm_align);
                    if (distance == 0xFFFFFFFFU) // marker found
                    {
                        rdec.normalize();
                        flush_data();
                        if (len == min_match_len) // End Of Stream marker
                        {
                            //              if( verify_trailer( pp ) ) return 0; else return 3;
                            return 0;
                        }
                        if (len == min_match_len + 1) // Sync Flush marker
                        {
                            rdec.load();
                            continue;
                        }
                        //            if( verbosity >= 0 )
                        //              {
                        //              pp();
                        std::fprintf(stderr, "Unsupported marker code '%d'\n", len);
                        //              }
                        return 4;
                    }
                }
            }
            rep3 = rep2;
            rep2 = rep1;
            rep1 = rep0;
            rep0 = distance;
            state.set_match();
            if (rep0 >= dictionary_size || (rep0 >= pos && !pos_wrapped))
            {
                flush_data();
                return 1;
            }
        }
        copy_block(rep0, len);
    }
    flush_data();
    return 2;
}
