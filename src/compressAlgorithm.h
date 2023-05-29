#ifndef COMPRESSALGORITHM_H_
#define COMPRESSALGORITHM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    void (*compress)(const uint8_t *inputStream, const int32_t inputStreamLength,
                     uint8_t *outputStream, int32_t *outputStreamLength);
    void (*decompress)(const uint8_t *inputStream, const int32_t inputStreamLength,
                       uint8_t *outputStream, int32_t *outputStreamLength);
} algorithmTypeDef;

extern algorithmTypeDef algorithmTable[];

#ifdef __cplusplus
}
#endif

#endif
