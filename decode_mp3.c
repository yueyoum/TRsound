#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpg123.h"
#include "sound.h"
#include "ao/ao.h"


static void cleanup(mpg123_handle *mh)
{
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}


int decode(const char *file)
{
    mpg123_handle *mh = NULL;
    unsigned char *buffer = NULL;
    size_t buffer_size = 0;
    size_t done = 0;
    int err = MPG123_OK;

    int channels = 0;
    int encoding = 0;
    long rate = 0;

    ao_device *device;

    err = mpg123_init();
    if(err != MPG123_OK || (mh = mpg123_new(NULL, &err)) == NULL)
    {
        fprintf(stderr, "setup error\n");
        cleanup(mh);
        exit(1);
    }


    if( mpg123_open(mh, file) != MPG123_OK ||
        mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK )
    {
        fprintf(stderr, "getformat error\n");
        cleanup(mh);
        exit(2);
    }


    //printf("rate = %ld\n", rate);
    //printf("channels = %d\n", channels);
    //printf("encoding_16: %d\n", encoding == MPG123_ENC_SIGNED_16);

    if(encoding != MPG123_ENC_SIGNED_16 && encoding != MPG123_ENC_FLOAT_32)
    {
        fprintf(stderr, "bad encoding\n");
        cleanup(mh);
        exit(3);
    }

    mpg123_format_none(mh);
    mpg123_format(mh, rate, channels, encoding);

    sound_init(channels, rate, &device);

    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char *)malloc(buffer_size * sizeof(char));

    //printf("buff_size = %u\n", buffer_size);


    while( mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK )
    {
        sound_speak((char *)buffer, buffer_size, device);
    }

    free(buffer);

    sound_shutdown(device);

    cleanup(mh);
    return 0;
}

#ifndef SHAREOBJ

int main(int argc, char *argv[])
{
    if( argc != 2)
    {
        fprintf(stderr, "need argument\n");
        exit(1);
    }

    decode(argv[1]);
    return 0;
}

#endif
