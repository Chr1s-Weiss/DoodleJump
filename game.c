/*
 * game.c
 *
 * Created: 23.05.2023 17:29:32
 *  Author: Christian and Clemens
 */
#include "game.h"
#include "../avrhal/display.h"
#include "../doodle-sprite.h"

uint16_t generateSeed()
{
    /* reading content of specific addresses from SRAM and doing bit operations to get larger value */
    uint8_t byte1 = *((uint8_t*)0x10);
    uint8_t byte2 = *((uint8_t*)0x15);
    uint16_t seedValue = (byte1 << 8) | byte2;

    return seedValue;
}

int16_t randMinMax(int16_t min, int16_t max)
{
    return (rand() % max) + min;
}

void initGame(Doodle* doodle, Bar bars[BAR_QUANTITY], int16_t* readjustY)
{
    doodle->score = 0;
    doodle->x = VERTICAL_DISPLAY_WIDTH / 2 - DOODLE_WIDTH / 2;
    doodle->y = 0;
    doodle->velocity = 4;
    doodle->gravity = -0.1;
    doodle->jumpValue = 1;
    doodle->falling = false;
    doodle->highScore = eeprom_read_word((uint16_t*)EEPROM_HIGH_SCORE_ADDR);
    *readjustY = 0;

    displayClearBuffer();
    generateBars(bars);
    displayUpdate();
}

void displayStartScreen()
{
    displayClearBuffer();
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 + FONT_HEIGHT * 2, 12, "chris");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 + FONT_HEIGHT, 4, "clemens");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT, 4, "present");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT * 2, 8, "doodle");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT * 3, 12, "jump");
    displayUpdate();
}

void generateBars(Bar bars[BAR_QUANTITY])
{
    int16_t lowerBarY = 0;

    for (int8_t i = 0; i < BAR_QUANTITY; ++i) {
        bars[i].x = randMinMax(0, (VERTICAL_DISPLAY_WIDTH - BAR_WIDTH));
        bars[i].y = randMinMax(BAR_MIN_DISTANCE + lowerBarY, BAR_MAX_DISTANCE);
        lowerBarY = bars[i].y - BAR_HEIGHT;
    }
}

void displayDoodle(Doodle* doodle)
{
    /* check if doodle is hitting the screen's x boundaries */
    if (doodle->x >= VERTICAL_DISPLAY_WIDTH - DOODLE_WIDTH) {
        doodle->x = VERTICAL_DISPLAY_WIDTH - DOODLE_WIDTH;
    }

    if (doodle->x <= 0) {
        doodle->x = 0;
    }
    /* check if doodle is hitting the screen's x boundaries */

    doodle->falling = doodle->velocity < 0 ? true : false;

    /* adjust doodle's velocity based on gravity; limit to -1 */
    if (doodle->velocity > -1) {
        doodle->velocity += doodle->gravity;
    } else {
        doodle->velocity = -1;
    }

    /* casting to int, because pixels (y-axis) can't be decimal */
    doodle->y += (int)doodle->velocity;

    Bitmap bitmap = imageDoodle();
    displayDrawBitmap(doodle->y, doodle->x, &bitmap);
}

void displayBars(Bar bars[BAR_QUANTITY])
{
    for (int8_t i = 0; i < BAR_QUANTITY; ++i) {
        displayDrawFilledRectangle(bars[i].y, bars[i].x, BAR_HEIGHT, BAR_WIDTH);
    }
}

void displayScore(Doodle* doodle)
{
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT - FONT_HEIGHT, 0, "%d", doodle->score);
}

bool doodleBarCollision(Doodle* doodle, Bar bars[BAR_QUANTITY], int16_t* readjustY)
{
    /* doodle is allowed the hit bars if going up */
    if (!doodle->falling)
        return false;

    for (int8_t i = 0; i < BAR_QUANTITY; ++i) {
        int16_t doodleRight = doodle->x + DOODLE_WIDTH;
        int16_t doodleBottom = doodle->y;
        int16_t barRight = bars[i].x + BAR_WIDTH;
        int16_t barBottom = bars[i].y + BAR_HEIGHT;

        if (doodle->x < barRight && doodleRight > bars[i].x && doodle->y < barBottom && doodleBottom > bars[i].y) {
            *readjustY = doodle->y;
            return true;
        }
    }
    return false;
}

void doodleJump(Doodle* doodle)
{
    doodle->velocity += 5;
}

void readjustDisplay(Doodle* doodle, Bar bars[BAR_QUANTITY], int16_t* readjustY)
{
    --(*readjustY);
    ++(doodle->score);

    --(doodle->y);
    for (int8_t i = 0; i < BAR_QUANTITY; ++i) {
        --bars[i].y;

        if (bars[i].y < 0) {
            bars[i].x = randMinMax(0, (VERTICAL_DISPLAY_WIDTH - BAR_WIDTH));
            /* bars[(i+5)%6] refers to the bar thats currently the highest on screen */
            bars[i].y = randMinMax(BAR_MIN_DISTANCE + bars[(i + 5) % 6].y, 15);
        }
    }
}

bool doodleHitGround(Doodle* doodle)
{
    /* check if doodle hits ground */
    return doodle->y <= 0;
}

void displayLoseScreen(Doodle* doodle)
{
    if (doodle->score > doodle->highScore) {
        doodle->highScore = doodle->score;
        eeprom_write_word((uint16_t*)EEPROM_HIGH_SCORE_ADDR, doodle->highScore);
    }

    displayClearBuffer();
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 + FONT_HEIGHT * 2, 0, "game");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 + FONT_HEIGHT, 0, "over");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT, 0, "h-score:");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT * 2, 0, "%d", doodle->highScore);
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT * 4, 0, "score:");
    displayPrintVertical(VERTICAL_DISPLAY_HEIGHT / 2 - FONT_HEIGHT * 5, 0, "%d", doodle->score);
    displayUpdate();
}
