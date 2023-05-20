#include "lzipWapper.h"
#include "string.h"
#include "stdio.h"

int main()
{
    uint8_t rawStream[64 * 4 * 4];
    uint8_t compressedStream[64 * 4 * 4];
    uint8_t decompressdStream[64 * 4 * 4] = {0};
    int32_t compressedStreamLength;
    int32_t decompressdStreamLength;
    lzipCompress(rawStream, 64 * 4, compressedStream, &compressedStreamLength);
    lzipDecompress(compressedStream, compressedStreamLength, decompressdStream, &decompressdStreamLength);
    if(memcmp(rawStream, decompressdStream, 64 * 4) == 0 && decompressdStreamLength == 64 * 4)
    {
        printf("lzipWapper test passed.\n");
    }
    else
    {
        printf("lzipWapper test failed.\n");
    }
    
    return 0;
}