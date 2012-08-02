#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED


#include "ao/ao.h"

void sound_speak(char *, const int, ao_device *);
void sound_init(const int, const long, ao_device **);
void sound_shutdown(ao_device *);



#endif // SOUND_H_INCLUDED
