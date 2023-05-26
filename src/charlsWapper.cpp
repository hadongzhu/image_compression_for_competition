#include "charlsWapper.h"
#include "charls.h"

uint8_t charlsHead[] = {0xff ,0xd8 ,0xff ,0xf7 ,0x00 ,0x14 ,0x08 ,0x00 ,0x08 ,0x00 ,0x08 ,0x04 ,0x01 ,0x11 ,0x00 ,0x02,
                        0x11 ,0x00 ,0x03 ,0x11 ,0x00 ,0x04 ,0x11 ,0x00 ,0xff ,0xda ,0x00 ,0x0e ,0x04 ,0x01 ,0x00 ,0x02,
                        0x00 ,0x03 ,0x00 ,0x04 ,0x00 ,0x00 ,0x02 ,0x00 ,};

uint8_t charlsTail[] = {0xff, 0xd9};

#define CHARLS_HEAD_LENGTH  (sizeof (charlsHead) / sizeof (uint8_t))
#define CHARLS_TAIL_LENGTH  (sizeof (charlsTail) / sizeof (uint8_t))

void charlsCompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                  uint8_t *outputStream, int32_t *outputStreamLength)
{
    charls::jpegls_encoder encoder;
    encoder.frame_info({ 8, 8, 8, 4 })
        .interleave_mode(charls::interleave_mode::sample)
        .near_lossless(0);
    std::vector<uint8_t> encoded_buffer(encoder.estimated_destination_size());
    encoder.destination(encoded_buffer);
    size_t encoded_size;
    encoded_size = encoder.encode(inputStream, 4);
    encoded_buffer.resize(encoded_size);
    *outputStreamLength = encoded_buffer.size() - CHARLS_HEAD_LENGTH - CHARLS_TAIL_LENGTH;
    memcpy(outputStream, (uint8_t *)encoded_buffer.data() + CHARLS_HEAD_LENGTH, *outputStreamLength);
}

void charlsDecompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                    uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempBuffer[500];
    memcpy(tempBuffer, charlsHead, CHARLS_HEAD_LENGTH);
    memcpy(tempBuffer + CHARLS_HEAD_LENGTH, inputStream, inputStreamLength);
    memcpy(tempBuffer + CHARLS_HEAD_LENGTH + inputStreamLength, charlsTail, CHARLS_TAIL_LENGTH);
    charls::jpegls_decoder decoder{ tempBuffer , (size_t)(CHARLS_HEAD_LENGTH + inputStreamLength + CHARLS_TAIL_LENGTH) , true };
    auto buffer = decoder.decode<std::vector<uint8_t>>();
    memcpy(outputStream, (void*)buffer.data(), (size_t)buffer.size());
    *outputStreamLength = buffer.size();
}