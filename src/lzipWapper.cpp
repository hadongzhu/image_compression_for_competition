#include "cstdio"
#include "sys/stat.h"
#include "lzipWapper.h"
#include "io.h"
#include "fcntl.h"
#include "lzip.h"

//+--+--+--+--+----+----+=============+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//| ID string | VN | DS | LZMA stream | CRC32 | Data size | Member size |
//+--+--+--+--+----+----+=============+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#define ID_STRING   0x4C, 0x5A, 0x49, 0x50
#define VN          0x01
#define DS          0x0C
#define DATA_SIZE   0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define MEMBER_SIZE 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

uint8_t lzipHead[] = {ID_STRING, VN, DS};
uint8_t lzipTail[] = {DATA_SIZE, MEMBER_SIZE};

static inline uint32_t lzipHeadSize()
{
    return sizeof(lzipHead) / sizeof (uint8_t);
}

static inline uint32_t lzipTailSize()
{
    return sizeof(lzipTail) / sizeof (uint8_t);
}

void lzipCompress(uint8_t *inputStream, uint32_t inputStreamLength,
                   uint8_t *outputStream, uint32_t *outputStreamLength)
{
    int infd = open("clzip_compress_temp", O_CREAT | O_RDWR | O_BINARY | O_TRUNC);
    write(infd, inputStream, inputStreamLength);
    close(infd);

    const char *argv[] = {"-9", "-f", "-k", "clzip_compress_temp"};
    int argc = sizeof (argv) / sizeof(char **);

    lzip(argc, (const char *const *) argv);

    int outfd = open("clzip_compress_temp.lz", O_RDONLY | O_BINARY);
    *outputStreamLength = lseek(outfd, 0, SEEK_END);
    lseek(outfd, 0, SEEK_SET);
    read(outfd, outputStream, *outputStreamLength);
    close(outfd);
    *outputStreamLength -= lzipHeadSize() + lzipTailSize();
    memcpy(outputStream, outputStream + lzipHeadSize(), *outputStreamLength);
    // remove("clzip_compress_temp.lz");
    // remove("clzip_compress_temp");
}

void lzipDecompress(uint8_t *inputStream, uint32_t inputStreamLength,
                     uint8_t *outputStream, uint32_t *outputStreamLength)
{
    int infd = open("clzip_decompress_temp.lz", O_CREAT | O_RDWR | O_BINARY | O_TRUNC);
    write(infd, lzipHead, lzipHeadSize());
    write(infd, inputStream, inputStreamLength);
    lzipTail[8] = inputStreamLength + lzipHeadSize() + lzipTailSize();
    write(infd, lzipTail, lzipTailSize());
    close(infd);

    const char* argv[] = {"-9", "-d", "-f", "-k", "clzip_decompress_temp.lz"};
    int argc = sizeof (argv) / sizeof(char **);

    lzip(argc, (const char *const *) argv);

    int outfd = open("clzip_decompress_temp", O_RDONLY | O_BINARY);
    *outputStreamLength = lseek(outfd, 0, SEEK_END);
    lseek(outfd, 0, SEEK_SET);
    read(outfd, outputStream, *outputStreamLength);
    close(outfd);
    // remove("clzip_decompress_temp");
    // remove("clzip_decompress_temp.lz");
}