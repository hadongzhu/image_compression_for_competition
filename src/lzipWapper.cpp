#include "cstdio"
#include "sys/stat.h"
#include "lzipWapper.h"
#include "io.h"
#include "fcntl.h"
#include "lzip.h"

void lzipCompress(uint8_t *inputStream, uint32_t inputStreamLength,
                   uint8_t *outputStream, uint32_t *outputStreamLength)
{
    int infd = open("clzip_compress_temp", O_CREAT | O_RDWR | O_BINARY | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
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
    remove("clzip_compress_temp.lz");
    remove("clzip_compress_temp");
}

void lzipDecompress(uint8_t *inputStream, uint32_t inputStreamLength,
                     uint8_t *outputStream, uint32_t *outputStreamLength)
{
    int infd = open("clzip_decompress_temp.lz", O_CREAT | O_RDWR | O_BINARY | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    write(infd, inputStream, inputStreamLength);
    close(infd);

    const char* argv[] = {"-9", "-d", "-f", "-k", "clzip_decompress_temp.lz"};
    int argc = sizeof (argv) / sizeof(char **);

    lzip(argc, (const char *const *) argv);

    int outfd = open("clzip_decompress_temp", O_RDONLY | O_BINARY);
    *outputStreamLength = lseek(outfd, 0, SEEK_END);
    lseek(outfd, 0, SEEK_SET);
    read(outfd, outputStream, *outputStreamLength);
    close(outfd);
    remove("clzip_decompress_temp");
    remove("clzip_decompress_temp.lz");
}