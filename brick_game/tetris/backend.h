#ifndef BACKEND_H
#define BACKEND_H

#include "../../gui/cli/frontend.h"
#include "../objects.h"

/** @file */

/*! \brief Перечисления состояний игры */
typedef enum game_state {
  /*! \brief Старт */
  START,
  /*! \brief Пауза */
  PAUSE,
  /*! \brief Появление фигуры */
  SPAWN,
  /*! \brief Движение */
  MOVING,
  /*! \brief Вращение */
  ROTATE,
  /*! \brief Выход из игры */
  EXIT_STATE,
  /*! \brief Проигрыш */
  GAMEOVER
} game_state;

/*! \brief Перечисления состояний ротаций фигур */
typedef enum rotate_state {
  /*! \brief Первое состояние */
  first_state,
  /*! \brief Второе состояние */
  second_state,
} rotate_state;

/*!
  \brief Функция пользовательсткого ввода

  \param UserAction_t пользовательский ввод

  \param hold заглушка
 */
void userInput(UserAction_t action, bool hold);

/*!
  \brief Функция для получения действия от пользователя

  \param user_input пользовательский ввод
 */
UserAction_t get_action(int user_input);

/*!
  \brief Функция инициализации игрового поля

  \param game_info поле
 */
void init_game_info(GameInfo_t *game_info);
/*!
  \brief Функция деинициализации игрового поля

  \param game_info поле
 */
void free_game_field(GameInfo_t *game_info);
/*!
  \brief Функция деинициализации игрового поля

  \param game_info поле
 */
void free_game_next(GameInfo_t *game_info);
/*!
  \brief Функция изменения игрового поля

  \param game_info поле
 */
GameInfo_t *change_game_info(GameInfo_t *game_info);
/*!
  \brief Функция получения игрового поля

  \param game_info поле
 */
GameInfo_t *get_game_info(GameInfo_t *gameInfo);
/*!
  \brief Функция игрового цикла

  \param game_info поле

  \param user_input пользовательский ввод
 */
game_state game_loop(GameInfo_t *game_info, UserAction_t user_input);
/*!
  \brief Функция получения игрового состояния

  \param game_state состояние игры
 */
game_state *get_state(game_state *state);
/*!
  \brief Функция получения текущей фигуры

  \param current фигура
 */
Tetramino *get_current(Tetramino *current);
/*!
  \brief Функция получения времени падения

  \param fall_time время падения
 */
bool *get_fall_time(bool *fall_time);
/*!
  \brief Функция получения следующей фигуры

  \param next_figure следующая фигура
 */
figure *get_next_figure(figure *next_figure);
/*!
  \brief Функция получения сигнала действия

  \param action действие

  \param fall_time время падения

  \param state состояние игры

  \param game_info информация о игре

  \param current текущая фигура

  \param next_figure следующая фигура
 */
void sigact(const UserAction_t *action, const bool *fall_time,
            game_state *state, GameInfo_t *game_info, Tetramino *current,
            figure *next_figure);
/*!
  \brief Функция состояния появления фигуры

  \param state состояние игры

  \param game_info информация о игре

  \param current текущая фигура

  \param next_figure следующая фигура
*/
void on_spawn_state(game_state *state, GameInfo_t *game_info,
                    Tetramino *current, figure *next_figure);
/*!
  \brief Функция состояния движения фигуры

  \param action действие

  \param game_info информация о игре

  \param current текущая фигура
*/
void on_moving_state(const UserAction_t *action, GameInfo_t *game_info,
                     Tetramino *current);
/*!
  \brief Функция инициализации фигуры I

  \param current текущая фигура
*/
void init_I(Tetramino *current);
/*!
  \brief Функция инициализации фигуры J

  \param current текущая фигура
*/
void init_J(Tetramino *current);
/*!
  \brief Функция инициализации фигуры L

  \param current текущая фигура
*/
void init_L(Tetramino *current);
/*!
  \brief Функция инициализации фигуры O

  \param current текущая фигура
*/
void init_O(Tetramino *current);
/*!
  \brief Функция инициализации фигуры S

  \param current текущая фигура
*/
void init_S(Tetramino *current);
/*!
  \brief Функция инициализации фигуры T

  \param current текущая фигура
*/
void init_T(Tetramino *current);
/*!
  \brief Функция инициализации фигуры Z

  \param current текущая фигура
*/
void init_Z(Tetramino *current);
/*!
  \brief Функция повления фигуры

  \param game_info информация о игре

  \param current_type тип фигуры

  \param current текущая фигура
 */
bool figure_spawn(const GameInfo_t *game_info, figure current_type,
                  Tetramino *current);
/*!
  \brief Функция проверки появления новой фигуры

  \param game_info информация о игре

  \param current текущая фигура
 */
bool is_can_spawn(const GameInfo_t *game_info, const Tetramino *current);
/*!
  \brief Функция обновления следующей фигуры

  \param game_info информация о игре

  \param next_current следующая фигура
 */
void next_figure_update(GameInfo_t *game_info, figure next_figure);
/*!
  \brief Функция инициализации следующей фигуры I

  \param game_info инфомрация об игре
*/
void next_init_I(GameInfo_t *game_info);
/*!
  \brief Функция инициализации следующей фигуры J

  \param game_info инфомрация об игре
*/
void next_init_J(GameInfo_t *game_info);
/*!
  \brief Функция инициализации следующей фигуры L

  \param game_info инфомрация об игре
*/
void next_init_L(GameInfo_t *game_info);
/*!
  \brief Функция инициализации следующей фигуры O

  \param game_info инфомрация об игре
*/
void next_init_O(GameInfo_t *game_info);
/*!
  \brief Функция инициализации следующей фигуры S

  \param game_info инфомрация об игре
*/
void next_init_S(GameInfo_t *game_info);
/*!
  \brief Функция инициализации следующей фигуры T

  \param game_info инфомрация об игре
*/
void next_init_T(GameInfo_t *game_info);
/*!
  \brief Функция инициализации следующей фигуры Z

  \param game_info инфомрация об игре
*/
void next_init_Z(GameInfo_t *game_info);
/*!
  \brief Функция обновлентия поля

  \param game_info информация о игре

  \param current текущая фигура
 */
void field_update(GameInfo_t *game_info, const Tetramino *current);
/*!
  \brief Функция стирания следов от движения фигуры

  \param game_info информация о игре

  \param current текущая фигура
 */
void clear_last_figure(GameInfo_t *game_info, const Tetramino *current);
/*!
  \brief Функция передвижения фигуры влево

  \param game_info информация о игре

  \param current текущая фигура
 */
void move_left(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция передвижения фигуры вправо

  \param game_info информация о игре

  \param current текущая фигура
 */
void move_right(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция передвижения фигуры вверх

  \param game_info информация о игре

  \param current текущая фигура
 */
void move_down(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры I

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_I(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры J

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_J(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры J

  \param game_info информация о игре

  \param current текущая фигура
 */
int rotate_J_first(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры J

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_J_second(const GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры L

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_L(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры L

  \param game_info информация о игре

  \param current текущая фигура
 */
int rotate_L_first(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры L

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_L_second(const GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры S

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_S(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры T

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_T(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры T

  \param game_info информация о игре

  \param current текущая фигура
 */
int rotate_T_first(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры T

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_T_second(const GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция вращения фигуры Z

  \param game_info информация о игре

  \param current текущая фигура
 */
void rotate_Z(GameInfo_t *game_info, Tetramino *current);
/*!
  \brief Функция проверки движения фигуры вниз

  \param game_info информация о игре

  \param current текущая фигура

  \param state состояние игры

  \param move_down может ли фигура двигаться вниз
 */
void check_move_down(GameInfo_t *game_info, const Tetramino *current,
                     game_state *state, int move_down);
/*!
  \brief Функция проверки линий

  \param game_info информация о игре
 */
int check_lines(GameInfo_t *game_info);
/*!
  \brief Функция проверки очков

  \param game_info информация о игре

  \param lines линии
 */
void check_score(GameInfo_t *gameInfo, int lines);

#endif