#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

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

class State
{
    int st;

public:
    enum
    {
        states = 12
    };
    State() : st(0) {}
    int operator()() const { return st; }
    bool is_char() const { return st < 7; }

    void set_char()
    {
        static const int next[states] = {0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 4, 5};
        st = next[st];
    }
    bool is_char_set_char()
    {
        set_char();
        return st < 4;
    }
    void set_char_rep() { st = 8; }
    void set_match() { st = (st < 7) ? 7 : 10; }
    void set_rep() { st = (st < 7) ? 8 : 11; }
    void set_short_rep() { st = (st < 7) ? 9 : 11; }
};

enum
{
    min_dictionary_bits = 12,
    min_dictionary_size = 1 << min_dictionary_bits, // >= modeled_distances
    max_dictionary_bits = 29,
    max_dictionary_size = 1 << max_dictionary_bits,
    min_member_size = 36,
    literal_context_bits = 3,
    literal_pos_state_bits = 0, // not used
    pos_state_bits = 2,
    pos_states = 1 << pos_state_bits,
    pos_state_mask = pos_states - 1,

    len_states = 4,
    dis_slot_bits = 6,
    start_dis_model = 4,
    end_dis_model = 14,
    modeled_distances = 1 << (end_dis_model / 2), // 128
    dis_align_bits = 4,
    dis_align_size = 1 << dis_align_bits,

    len_low_bits = 3,
    len_mid_bits = 3,
    len_high_bits = 8,
    len_low_symbols = 1 << len_low_bits,
    len_mid_symbols = 1 << len_mid_bits,
    len_high_symbols = 1 << len_high_bits,
    max_len_symbols = len_low_symbols + len_mid_symbols + len_high_symbols,

    min_match_len = 2,                                   // must be 2
    max_match_len = min_match_len + max_len_symbols - 1, // 273
    min_match_len_limit = 5
};

inline int get_len_state(const int len)
{
    return std::min(len - min_match_len, len_states - 1);
}

inline int get_lit_state(const uint8_t prev_byte)
{
    return prev_byte >> (8 - literal_context_bits);
}

enum
{
    bit_model_move_bits = 5,
    bit_model_total_bits = 11,
    bit_model_total = 1 << bit_model_total_bits
};

struct Bit_model
{
    int probability;
    void reset() { probability = bit_model_total / 2; }
    void reset(const int size)
    {
        for (int i = 0; i < size; ++i)
            this[i].reset();
    }
    Bit_model() { reset(); }
};

struct Len_model
{
    Bit_model choice1;
    Bit_model choice2;
    Bit_model bm_low[pos_states][len_low_symbols];
    Bit_model bm_mid[pos_states][len_mid_symbols];
    Bit_model bm_high[len_high_symbols];

    void reset()
    {
        choice1.reset();
        choice2.reset();
        bm_low[0][0].reset(pos_states * len_low_symbols);
        bm_mid[0][0].reset(pos_states * len_mid_symbols);
        bm_high[0].reset(len_high_symbols);
    }
};

class CRC32
{
    uint32_t data[256]; // Table of CRCs of all 8-bit messages.

public:
    CRC32()
    {
        for (unsigned n = 0; n < 256; ++n)
        {
            unsigned c = n;
            for (int k = 0; k < 8; ++k)
            {
                if (c & 1)
                    c = 0xEDB88320U ^ (c >> 1);
                else
                    c >>= 1;
            }
            data[n] = c;
        }
    }

    uint32_t operator[](const uint8_t byte) const { return data[byte]; }

    void update_byte(uint32_t &crc, const uint8_t byte) const
    {
        crc = data[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    }

    // about as fast as it is possible without messing with endianness
    void update_buf(uint32_t &crc, const uint8_t *const buffer,
                    const int size) const
    {
        uint32_t c = crc;
        for (int i = 0; i < size; ++i)
            c = data[(c ^ buffer[i]) & 0xFF] ^ (c >> 8);
        crc = c;
    }
};

extern const CRC32 crc32;

inline int real_bits(unsigned value)
{
    int bits = 0;
    while (value > 0)
    {
        value >>= 1;
        ++bits;
    }
    return bits;
}
