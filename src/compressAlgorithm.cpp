#include <cstring>
#include "lzipWapper.h"
#include "charlsWapper.h"
#include "compressAlgorithm.h"

void charlsAndLzipCompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                    uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    charlsCompress(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    lzipCompress(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void charlsAndLzipDecompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                      uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    lzipDecompress(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    charlsDecompress(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void blankCompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                           uint8_t *outputStream, int32_t *outputStreamLength)
{
    for(int32_t i = 0; i < inputStreamLength; i++)
    {
        if(inputStream[i] != 0)
        {
            *outputStreamLength = 256;
            return;
        }
    }
    *outputStreamLength = 0;
}

void blankDecompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                   uint8_t *outputStream, int32_t *outputStreamLength)
{
    *outputStreamLength = 256;
    memset(outputStream, 0x00, *outputStreamLength);
}

algorithmTypeDef algorithmTable[] = {
            {0x00, NULL, NULL},
            {0x01, charlsCompress, charlsDecompress},
            {0x02, lzipCompress, lzipDecompress},
            {0x03, charlsAndLzipCompress, charlsAndLzipDecompress},
            {0x04, blankCompress, blankDecompress},
};