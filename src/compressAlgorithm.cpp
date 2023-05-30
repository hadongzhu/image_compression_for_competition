#include <cstring>
#include "lzipWapper.h"
#include "charlsWapper.h"
#include "compressAlgorithm.h"
#include "YUVConvert.h"

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

void charlsAndYUVCompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                           uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    BGRA2VUYA(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    charlsCompress(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void charlsAndYUVDecompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                          uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    charlsDecompress(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    VUYA2BGRA(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void charlsAndColor1Compress(const uint8_t *inputStream, const int32_t inputStreamLength,
                          uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    BGRAToColor1(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    charlsCompress(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void charlsAndColor1Decompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                            uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    charlsDecompress(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    color1ToBGRA(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void charlsAndColor2Compress(const uint8_t *inputStream, const int32_t inputStreamLength,
                             uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    BGRAToColor2(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    charlsCompress(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

void charlsAndColor2Decompress(const uint8_t *inputStream, const int32_t inputStreamLength,
                               uint8_t *outputStream, int32_t *outputStreamLength)
{
    uint8_t tempStream[64 * 4 * 4 * 2];
    int32_t tempStreamLength;
    charlsDecompress(inputStream, inputStreamLength, tempStream, &tempStreamLength);
    color2ToBGRA(tempStream, tempStreamLength, outputStream, outputStreamLength);
}

algorithmTypeDef algorithmTable[] = {
            {NULL, NULL},
            {charlsCompress, charlsDecompress},
            {lzipCompress, lzipDecompress},
//            {charlsAndLzipCompress, charlsAndLzipDecompress},
            {blankCompress, blankDecompress},
            {charlsAndYUVCompress, charlsAndYUVDecompress},
            {charlsAndColor1Compress, charlsAndColor1Decompress},
            {charlsAndColor2Compress, charlsAndColor2Decompress}
};