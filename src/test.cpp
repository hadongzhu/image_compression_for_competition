#include "lzipWapper.h"
#include "charlsWapper.h"
#include "string.h"
#include "stdio.h"

int main()
{
    uint8_t rawStream[64 * 4 * 4];
    uint8_t charlsStream[64 * 4 * 4];
    int32_t charlsStreamLength;
    uint8_t compressedStream[64 * 4 * 4];
    uint8_t decompressdStream[64 * 4 * 4] = {0};
    int32_t compressedStreamLength;
    int32_t decompressdStreamLength;
    charlsCompress(rawStream, 64 * 4, charlsStream, &charlsStreamLength);
    //lzipCompress(rawStream, 64 * 4, compressedStream, &compressedStreamLength);
    lzipCompress(charlsStream, charlsStreamLength, compressedStream, &compressedStreamLength);

    //lzipDecompress(compressedStream, compressedStreamLength, decompressdStream, &decompressdStreamLength);
    lzipDecompress(compressedStream, compressedStreamLength, charlsStream, &charlsStreamLength);
    charlsDecompress(charlsStream, charlsStreamLength, decompressdStream, &decompressdStreamLength);

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