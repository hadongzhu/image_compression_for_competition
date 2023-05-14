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

void lzipCompress(uint8_t *inputStream, uint32_t inputStreamLength,
                   uint8_t *outputStream, uint32_t *outputStreamLength);

void lzipDecompress(uint8_t *inputStream, uint32_t inputStreamLength,
                   uint8_t *outputStream, uint32_t *outputStreamLength);

#ifdef __cplusplus
}
#endif

#endif
