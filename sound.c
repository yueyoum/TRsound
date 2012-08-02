#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ao/ao.h"
#include "sound.h"



void sound_init(const int channels, const long rate, ao_device **device)
{
    int default_driver;
    ao_sample_format format;

    ao_initialize();

    default_driver = ao_default_driver_id();
    memset(&format, 0, sizeof(format));

    format.bits = 16;
    format.channels = channels;
    format.rate = rate;
    format.byte_format = AO_FMT_NATIVE;
    //format.maxtix = 0;

    //printf("default_driver = %d\n", default_driver);
    //printf("rate = %d\n", format.rate);

    *device = ao_open_live(default_driver, &format, NULL);
    if((*device) == NULL)
    {
        fprintf(stderr, "Error opening device\n");
        exit(-1);
    }

    return;
}

void sound_shutdown(ao_device *device)
{
    ao_close(device);
    ao_shutdown();
    return;
}


void sound_speak(char *data, const int length, ao_device *device)
{
    ao_play(device, data, length);
    return;
}
