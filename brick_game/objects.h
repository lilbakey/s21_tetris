#ifndef OBJECTS_H
#define OBJECTS_H

/** @file */

/*! \brief Структура с фигурами */
typedef enum figure { I = 1, J, L, O, S, T, Z } figure;

/*! \brief Структура фигуры */
typedef struct Tetramino {
  /*! \brief Координата х1 */
  int x1;
  /*! \brief Координата у1 */
  int y1;

  /*! \brief Координата х2 */
  int x2;
  /*! \brief Координата у2 */
  int y2;

  /*! \brief Координата х3 */
  int x3;
  /*! \brief Координата у3 */
  int y3;

  /*! \brief Координата х4 */
  int x4;
  /*! \brief Координата у4 */
  int y4;

  /*! \brief Тип фигуры */
  figure type;
  /*! \brief Состояние ротации */
  int rotate_state;
  /*! \brief Цвет */
  int color;
  /*! \brief Возможность двигаться вниз */
  int move_down;
} Tetramino;

/*! \brief Структура с возможными действиями */
typedef enum UserAction_t {
  /*! \brief Начата ли игра */
  Start,
  /*! \brief Поставлена ли сейчас пауза в игре */
  Pause,
  /*! \brief Нажата ли кнопка завершения игры */
  Terminate,
  /*! \brief Поворот влево */
  Left,
  /*! \brief Поворот вправо */
  Right,
  /*! \brief Действие вверх (в тетрисе не активна) */
  Up,
  /*! \brief Спуск фигуры вниз */
  Down,
  /*! \brief Вращение */
  Action
} UserAction_t;

/*! \brief Структура с информацией об игре */
typedef struct GameInfo_t {
  /*! \brief Поле */
  int **field;
  /*! \brief Следующая фигура */
  int **next;
  /*! \brief Счет */
  int score;
  /*! \brief Максимальный счет */
  int high_score;
  /*! \brief Уровень */
  int level;
  /*! \brief Скорость */
  int speed;
  /*! \brief Пауза */
  int pause;
} GameInfo_t;

#endif