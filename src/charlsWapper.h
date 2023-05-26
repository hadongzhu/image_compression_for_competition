#ifndef CHARLSWAPPER_H_
#define CHARLSWAPPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

void charlsCompress(const uint8_t *inputStream, int32_t inputStreamLength,
                   uint8_t *outputStream, int32_t *outputStreamLength);

void charlsDecompress(const uint8_t *inputStream, int32_t inputStreamLength,
                   uint8_t *outputStream, int32_t *outputStreamLength);

#ifdef __cplusplus
}
#endif

#endif
