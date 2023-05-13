/* clzipWapper.h
*  clzip wapper
*/
#ifndef _CLZIPWAPPER_H_
#define _CLZIPWAPPER_H_

#include "stdint.h"

void clzipCompress(uint8_t *inputStream, uint32_t inputStreamLength,
                   uint8_t *outputStream, uint32_t outputStreamLength);

void clzipDecompress(uint8_t *inputStream, uint32_t inputStreamLength,
                   uint8_t *outputStream, uint32_t outputStreamLength);

#endif
