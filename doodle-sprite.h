/*
 * doodleSprite.h
 *
 * Created: 05/06/2023 13:21:49
 *  Author: Christian and Clemens
 */

#ifndef DOODLESPRITE_H_
#define DOODLESPRITE_H_

#include <avr/pgmspace.h>
#include <stdint.h>

#include "avrhal/bitmap.h"
#include "game/game.h"

/** Created by https://jinxedbyte.itch.io/1bit-dog-puppy
 *  Converted with the LCD-Image-Converter for the Microcontroller Lab at the Salzburg University of Applied Sciences
 *  Converter Settings: Monochrome, Main Scan Direction: left to right, Line Scan Direction: backwards 
 */
static Bitmap imageDoodle()
{
    /* To save space, the font is stored in program memory (flash) */
    /* See https://www.nongnu.org/avr-libc/user-manual/pgmspace.html for further details. */

    static const uint8_t bitmap[8] PROGMEM = {
        0b01000010,
        0b01000010,
        0b11111111,
        0b10000001,
        0b10000001,
        0b10100101,
        0b10000001,
        0b01111110
    };

    return (Bitmap){ .data = bitmap, .width = DOODLE_WIDTH, .height = DOODLE_HEIGHT, .dataSize = 8 };
}

#endif /* DOODLESPRITE_H_ */
