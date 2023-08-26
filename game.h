/*
 * game.h
 *
 * Created: 23.05.2023 17:29:22
 *  Author: Christian and Clemens
 */

#ifndef GAME_H_
#define GAME_H_

#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>

#define EEPROM_HIGH_SCORE_ADDR 0x00

#define VERTICAL_DISPLAY_WIDTH DISPLAY_HEIGHT
#define VERTICAL_DISPLAY_HEIGHT DISPLAY_WIDTH
#define FONT_HEIGHT 8

#define DELAY_START_SCREEN 3000
#define DELAY_LOSE_SCREEN 1500

#define DOODLE_WIDTH 8
#define DOODLE_HEIGHT 8

#define BAR_QUANTITY 6
#define BAR_WIDTH 12
#define BAR_HEIGHT 2
#define BAR_MIN_DISTANCE 20
#define BAR_MAX_DISTANCE 15

typedef struct {
    int16_t x;
    int16_t y;
    int16_t highScore;
    int16_t score;

    double velocity;
    double gravity;
    double jumpValue;

    bool falling;
} Doodle;

typedef struct {
    int16_t x;
    int16_t y;
} Bar;

uint16_t generateSeed();
int16_t randMinMax(int16_t min, int16_t max);
void initGame(Doodle* doodle, Bar bars[BAR_QUANTITY], int16_t* readjustY);
void displayStartScreen();
void generateBars(Bar bars[BAR_QUANTITY]);
void displayDoodle(Doodle* doodle);
void displayBars(Bar bars[BAR_QUANTITY]);
void displayScore(Doodle* doodle);
bool doodleBarCollision(Doodle* doodle, Bar bars[BAR_QUANTITY], int16_t* readjustY);
void doodleJump(Doodle* doodle);
void readjustDisplay(Doodle* doodle, Bar bars[BAR_QUANTITY], int16_t* readjustY);
bool doodleHitGround(Doodle* doodle);
void displayLoseScreen(Doodle* doodle);

#endif /* GAME_H_ */
