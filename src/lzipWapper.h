/* clzipWapper.h
*  clzip wapper
*/
#ifndef CLZIPWAPPER_H_
#define CLZIPWAPPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

void lzipCompress(const uint8_t *inputStream, int32_t inputStreamLength,
                   uint8_t *outputStream, int32_t *outputStreamLength);

void lzipDecompress(const uint8_t *inputStream, int32_t inputStreamLength,
                   uint8_t *outputStream, int32_t *outputStreamLength);

#ifdef __cplusplus
}
#endif

#endif
