#include "lzipWapper.h"
#include "lzip.h"
#include "decoder.h"
#include "encoder_base.h"
#include "encoder.h"

#include "charls.h"


struct Lzma_options
{
    int dictionary_size; // 4 KiB .. 512 MiB
    int match_len_limit; // 5 .. 273
};

const Lzma_options option_mapping[] =
    {
        {1 << 16, 16},   // -0
        {1 << 20, 5},    // -1
        {3 << 19, 6},    // -2
        {1 << 21, 8},    // -3
        {3 << 20, 12},   // -4
        {1 << 22, 20},   // -5
        {1 << 23, 36},   // -6
        {1 << 24, 68},   // -7
        {3 << 23, 132},  // -8
        {1 << 25, 273}}; // -9

void lzipCompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                  uint8_t *outputStream, int32_t *outputStreamLength)
{
    dis_slots.init();
    prob_prices.init();
    Lzma_options encoder_options = option_mapping[6];
    LZ_encoder_base *encoder = 0; // polymorphic encoder
    encoder = new LZ_encoder(encoder_options.dictionary_size,
                             encoder_options.match_len_limit, inputStream, inputStreamLength, outputStream, outputStreamLength);
    while (true) // encode one member per iteration
    {
        encoder->encode_member(0x0008000000000000ULL);
        if (encoder->data_finished())
            break;
        encoder->reset();
    }
    delete encoder;
}

void lzipDecompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                    uint8_t *outputStream, int32_t *outputStreamLength)
{
    Range_decoder rdec(inputStream, inputStreamLength);
    rdec.reset_member_position();
    LZ_decoder decoder(rdec, 4096, outputStream, outputStreamLength);
    decoder.decode_member();
}