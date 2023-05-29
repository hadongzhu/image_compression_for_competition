#ifndef YUVCONVERT_H_
#define YUVCONVERT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"
typedef struct
{
    uint8_t V;
    uint8_t U;
    uint8_t Y;
    uint8_t A;
} VUYAStructDef;

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
} BGRAStructDef;

void YUVTransform(BGRAStructDef *BGRA, VUYAStructDef *VUYA);
void YUVInverseTransform(VUYAStructDef *VUYA, BGRAStructDef *BGRA);

void colorTransform1(BGRAStructDef *BGRA, VUYAStructDef *VUYA);
void colorInverseTransform1(VUYAStructDef *VUYA, BGRAStructDef *BGRA);

void colorTransform2(BGRAStructDef *BGRA, VUYAStructDef *VUYA);
void colorInverseTransform2(VUYAStructDef *VUYA, BGRAStructDef *BGRA);

void colorTransform(const uint8_t *inputStream, const int32_t inputStreamLength,
                    uint8_t *outputStream, int32_t *outputStreamLength, void(*colorTransform)(BGRAStructDef *, VUYAStructDef *));

void colorInverseTransform(const uint8_t *inputStream, const int32_t inputStreamLength,
                           uint8_t *outputStream, int32_t *outputStreamLength, void(*colorInverseTransform)(VUYAStructDef *, BGRAStructDef *));

static inline void BGRA2VUYA(const uint8_t *inputStream, int32_t inputStreamLength,
                             uint8_t *outputStream, int32_t *outputStreamLength)
{
    colorTransform(inputStream, inputStreamLength, outputStream, outputStreamLength, YUVTransform);
}

static inline void VUYA2BGRA(const uint8_t *inputStream, int32_t inputStreamLength,
                             uint8_t *outputStream, int32_t *outputStreamLength)
{
    colorInverseTransform(inputStream, inputStreamLength, outputStream, outputStreamLength, YUVInverseTransform);
}

static inline void BGRAToColor1(const uint8_t *inputStream, int32_t inputStreamLength,
                              uint8_t *outputStream, int32_t *outputStreamLength)
{
    colorTransform(inputStream, inputStreamLength, outputStream, outputStreamLength, colorTransform1);
}

static inline void color1ToBGRA(const uint8_t *inputStream, int32_t inputStreamLength,
                               uint8_t *outputStream, int32_t *outputStreamLength)
{
    colorInverseTransform(inputStream, inputStreamLength, outputStream, outputStreamLength, colorInverseTransform1);
}

static inline void BGRAToColor2(const uint8_t *inputStream, int32_t inputStreamLength,
                              uint8_t *outputStream, int32_t *outputStreamLength)
{
    colorTransform(inputStream, inputStreamLength, outputStream, outputStreamLength, colorTransform2);
}

static inline void color2ToBGRA(const uint8_t *inputStream, int32_t inputStreamLength,
                               uint8_t *outputStream, int32_t *outputStreamLength)
{
    colorInverseTransform(inputStream, inputStreamLength, outputStream, outputStreamLength, colorInverseTransform2);
}

#ifdef __cplusplus
}
#endif

#endif
