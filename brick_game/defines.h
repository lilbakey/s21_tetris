#ifndef DEFINES_H
#define DEFINES_H

/** @file */

/*!

  \brief Клавиша Enter

 */
#define ACTION_KEY 10
/*!

  \brief Клавиша ESCAPE

 */
#define ESCAPE_KEY 27

/*!
 * \brief Значение начала доски
 */
#define BOARDS_BEGIN 2
/*!
 * \brief Высота поля
 */
#define HEIGHT 20

/*!
 * \brief Ширина поля
 */
#define WIDTH 10

/*!
 * \brief Значение для отрисовки поля
 */
#define HUD_WIDTH 12

/*!
 * \brief Значение для отрисовки поля
 */
#define MAP_PADDING 3

/*!
 * \brief Значение для успешного выполнения
 */
#define SUCCESS 0

/*!
 * \brief Макрос для отрисовки поля
 */
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

/*!
 * \brief Макрос для отрисовки поля
 */
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)

#endif