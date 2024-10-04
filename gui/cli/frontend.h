#ifndef FRONTEND_H
#define FRONTEND_H

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../brick_game/defines.h"
#include "../../brick_game/objects.h"

/** @file */

/*!
  \brief Функция отрисовки выбора игры

  \param game номер игры
 */
void select_game(int game);

/*!
  \brief Функция отрисовки прямоугольника

  \param top_y координата

  \param bottom_y координата

  \param left_x координата

  \param right_x координата
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/*!
  \brief Функция отрисовки оверлея

  \param void
 */
void print_overlay(void);

/*!
  \brief Функция отрисовки конца игры

  \param void
 */
void print_gameover(void);

/*!
  \brief Функция обновления текущего состояния игры

  \param void
 */
GameInfo_t updateCurrentState(void);

/*!
  \brief Функция отрисовки поля

  \param GameInfo_t поле

  \param Tetramino фигура
 */
void print_field(const GameInfo_t *field, Tetramino *current);

/*!
  \brief Функция отрисовки статистики на поле

  \param GameInfo_t статы
 */
void print_stats(GameInfo_t *stats);

/*!
  \brief Функция отрисовки селующей фигуры

  \param GameInfo_t следующая фигура
 */
void print_next(const GameInfo_t *next);

/*!
  \brief Функция инифиализации цветов ncurses

  \param void
 */
void init_color_pairs(void);

#endif
