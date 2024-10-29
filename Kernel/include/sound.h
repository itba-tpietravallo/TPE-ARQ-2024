#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdint.h>

void beep(uint32_t nFrequence, uint64_t sleep_t);

enum SPEAKER {
    SPEAKER_ON = 0xFF,
    SPEAKER_OFF = 0x00,
};

void setPITMode(uint8_t mode);
void setPITFrequency(uint16_t freq);
void setSpeaker(enum SPEAKER status);

#endif