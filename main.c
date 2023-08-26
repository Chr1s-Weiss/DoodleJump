/*
 * doodle-jump.c
 *
 * Created: 23.05.2023 16:57:55
 * Author : Christian and Clemens
 */

#include "avrhal/adc.h"
#include "avrhal/display.h"
#include "avrhal/usart.h"
#include "game/game.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

/*
	NOTE: This application is played and displayed in portrait mode,
	therefore the axis are swapped. This must be taken into account
	when calling functions from the avrhal directory.
*/

int main(void)
{
    adcSetup();
    sei();
    displaySetup();

    srand(generateSeed());

    /* using local variables instead of pointers to determine memory usage */
    Doodle doodle;
    Bar bars[BAR_QUANTITY];
    int16_t readjustY;

    initGame(&doodle, bars, &readjustY);
    displayStartScreen();
    _delay_ms(DELAY_START_SCREEN);

    volatile int16_t adcX;
    while (1) {
        displayClearBuffer();

        adcSetChannel(0);
        adcX = adcRead();
        /* adcX range: 1023 (left) - 0 (right); adjusting range to VERTICAL_DISPLAY_WIDTH; inverting directions by 64 - ...  */
        doodle.x = (64 - (adcX / 16 + 1)) - DOODLE_WIDTH / 2;

        displayDoodle(&doodle);
        displayBars(bars);
        displayScore(&doodle);

        if (doodleBarCollision(&doodle, bars, &readjustY)) {
            doodleJump(&doodle);
        }

        if (readjustY > 1) {
            readjustDisplay(&doodle, bars, &readjustY);
        }

        if (doodleHitGround(&doodle)) {
            displayLoseScreen(&doodle);
            _delay_ms(DELAY_LOSE_SCREEN);
            initGame(&doodle, bars, &readjustY);
        }

        displayUpdate();
    }

    return 0;
}
