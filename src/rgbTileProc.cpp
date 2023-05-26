/* rgbTileProc.cpp
*  implementation of TILE COMPRESSION
*/
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <assert.h>
#include "rgbTileProc.h"

#include "lzipWapper.h"
#include "charlsWapper.h"

static int g_nTileWidth = 0;
static int g_nTileHeight = 0;

void tileSetSize(int nTileWidth, int nTileHeight)
{
	g_nTileWidth = nTileWidth;
	g_nTileHeight = nTileHeight;
}

/* compress ARGB data to tile
*  param:
*    pClrBlk      -- IN, pixel's ARGB data
*    pTile        -- OUT, tile data
*    pTileSize    -- OUT, tile's bytes
*  return:
*    0  -- succeed
*   -1  -- failed
*/
int argb2tile(const unsigned char* pClrBlk, unsigned char* pTile, int* pTileSize)
{
	assert(g_nTileWidth > 0 && g_nTileHeight > 0);
//	*pTileSize = g_nTileWidth * g_nTileHeight * 4;
//	memcpy(pTile, pClrBlk, *pTileSize);
	uint8_t tempStream[64 * 4 * 4];
	int32_t tempStreamLength;

    charlsCompress((unsigned char*)pClrBlk, g_nTileWidth * g_nTileHeight * 4, pTile, pTileSize);
//	charlsCompress((unsigned char*)pClrBlk, g_nTileWidth * g_nTileHeight * 4, tempStream, &tempStreamLength);
//	lzipCompress(tempStream, tempStreamLength, pTile, pTileSize);

    //lzipCompress((unsigned char*)pClrBlk, g_nTileWidth * g_nTileHeight * 4, pTile, pTileSize);
	return 0;
}

/* decompress tile data to ARGB
*  param:
*    pTile        -- IN, tile data
*    pTileSize    -- IN, tile's bytes
*    pClrBlk      -- OUT, pixel's ARGB data
*  return:
*    0  -- succeed
*   -1  -- failed
*/
int tile2argb(const unsigned char* pTile, int nTileSize, unsigned char* pClrBlk)
{
//	memcpy(pClrBlk, pTile, nTileSize);
    int32_t nClrBlkSize;
	uint8_t tempStream[64 * 4 * 4];
	int32_t tempStreamLength;

    charlsDecompress((unsigned char*)pTile, nTileSize, pClrBlk, &nClrBlkSize);

//	lzipDecompress(pTile, nTileSize, tempStream, &tempStreamLength);
//	charlsDecompress(tempStream, tempStreamLength, (unsigned char*)pClrBlk, &nClrBlkSize);

    //lzipDecompress((unsigned char*)pTile, nTileSize, pClrBlk, &nClrBlkSize);
	return 0;
}
