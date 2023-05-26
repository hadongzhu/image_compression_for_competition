#include "charlsWapper.h"
#include "charls.h"

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
    memcpy(outputStream, (void*)encoded_buffer.data(), (size_t)encoded_buffer.size());
    *outputStreamLength = encoded_buffer.size();
}

void charlsDecompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                    uint8_t *outputStream, int32_t *outputStreamLength)
{
    charls::jpegls_decoder decoder{ inputStream , (size_t)inputStreamLength , true };
    auto buffer = decoder.decode<std::vector<uint8_t>>();
    memcpy(outputStream, (void*)buffer.data(), (size_t)buffer.size());
    *outputStreamLength = buffer.size();
}