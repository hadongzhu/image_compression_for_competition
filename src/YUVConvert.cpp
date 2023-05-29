#include <cmath>
#include "YUVConvert.h"

void colorTransform(const uint8_t *inputStream, const int32_t inputStreamLength,
                  uint8_t *outputStream, int32_t *outputStreamLength, void(*colorTransform)(BGRAStructDef *, VUYAStructDef *))
{
    for(int32_t index = 0; index < inputStreamLength; index += 4)
    {
        colorTransform((BGRAStructDef *)&inputStream[index], (VUYAStructDef *)&outputStream[index]);
    }
    *outputStreamLength = inputStreamLength;
}

void colorInverseTransform(const uint8_t *inputStream, const int32_t inputStreamLength,
                    uint8_t *outputStream, int32_t *outputStreamLength, void(*colorInverseTransform)(VUYAStructDef *, BGRAStructDef *))
{
    for(int32_t index = 0; index < inputStreamLength; index += 4)
    {
        colorInverseTransform((VUYAStructDef *)&inputStream[index], (BGRAStructDef *)&outputStream[index]);
    }
    *outputStreamLength = inputStreamLength;
}

void YUVTransform(BGRAStructDef *BGRA, VUYAStructDef *VUYA)
{
    VUYA->V = BGRA->B - BGRA->G + 128;
    VUYA->U = BGRA->R - BGRA->G + 128;
    VUYA->Y = BGRA->G + (VUYA->V + VUYA->U) / 4 - 64;
    VUYA->A = BGRA->A;
}

void YUVInverseTransform(VUYAStructDef *VUYA, BGRAStructDef *BGRA)
{
    BGRA->G = VUYA->Y - (VUYA->U + VUYA->V) / 4 + 64;
    BGRA->R = VUYA->U + BGRA->G - 128;
    BGRA->B = VUYA->V + BGRA->G - 128;
    BGRA->A = VUYA->A;
}

void colorTransform1(BGRAStructDef *BGRA, VUYAStructDef *VUYA)
{
    VUYA->V = BGRA->G;
    VUYA->U = BGRA->B - BGRA->G + 128;
    VUYA->Y = BGRA->R - BGRA->G + 128;
    VUYA->A = BGRA->A;
}

void colorInverseTransform1(VUYAStructDef *VUYA, BGRAStructDef *BGRA)
{
    BGRA->G = VUYA->V;
    BGRA->R = VUYA->Y + VUYA->V - 128;
    BGRA->B = VUYA->U + VUYA->V - 128;
    BGRA->A = VUYA->A;
}

void colorTransform2(BGRAStructDef *BGRA, VUYAStructDef *VUYA)
{
    VUYA->V = BGRA->G;
    VUYA->U = BGRA->B - (BGRA->R + BGRA->G) / 2 - 128;
    VUYA->Y = BGRA->R - BGRA->G + 128;
    VUYA->A = BGRA->A;
}

void colorInverseTransform2(VUYAStructDef *VUYA, BGRAStructDef *BGRA)
{
    BGRA->G = VUYA->V;
    BGRA->R = VUYA->Y + VUYA->V - 128;
    BGRA->B = VUYA->U + (BGRA->R + VUYA->V) / 2 - 128;
    BGRA->A = VUYA->A;
}