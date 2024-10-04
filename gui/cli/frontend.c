#include "frontend.h"

#include "../../brick_game/tetris/backend.h"

/** @file */

void select_game(int game) {
  MVPRINTW(4, 3, "         {MENU}  ");
  MVPRINTW(6, 3, "    1. Tetris");
  MVPRINTW(8, 3, "    2. Snake (NOT READY)");
  MVPRINTW(10, 3, "    3. Racer (NOT READY)");
  switch (game) {
    case 0:
      MVPRINTW(6, 4, "->");
      break;
    case 1:
      MVPRINTW(8, 4, "-> ");
      break;
    case 2:
      MVPRINTW(10, 4, "-> ");
      break;
    default:
      break;
  }
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_overlay() {
  print_rectangle(0, HEIGHT, 0, HEIGHT + 1);
  print_rectangle(0, HEIGHT, HEIGHT + 2, HEIGHT + HUD_WIDTH + 3);

  print_rectangle(1, 5, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(6, 8, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(9, 12, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(13, 16, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(17, 19, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);

  MVPRINTW(2, HEIGHT + 5, " NEXT");
  MVPRINTW(7, HEIGHT + 5, " LEVEL %d", 0);
  MVPRINTW(10, HEIGHT + 5, " SCORE");
  MVPRINTW(14, HEIGHT + 5, "HI_SCORE");
  MVPRINTW(18, HEIGHT + 5, "SPEED %d", 1);
  MVPRINTW(22, 1, "Enter  - START & ROTATE");
  MVPRINTW(23, 1, "Arrows - MOVE");
  MVPRINTW(24, 1, "P      - PAUSE");
  MVPRINTW(25, 1, "ESC    - EXIT");
}

void print_gameover() {
  print_rectangle(0, HEIGHT, 0, HEIGHT + 1);
  print_rectangle(0, HEIGHT, HEIGHT + 2, HEIGHT + HUD_WIDTH + 3);

  print_rectangle(1, 8, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(9, 11, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(12, 15, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);
  print_rectangle(16, 19, HEIGHT + 3, HEIGHT + HUD_WIDTH + 2);

  MVPRINTW(5, 5, "GAME OVER");
  MVPRINTW(22, 1, "Enter  - RESTART & ROTATE");
  MVPRINTW(23, 1, "Arrows - MOVE");
  MVPRINTW(24, 1, "P      - PAUSE");
  MVPRINTW(25, 1, "ESC    - EXIT");
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game_info = change_game_info(NULL);
  Tetramino *current = get_current(NULL);
  print_field(game_info, current);
  print_next(game_info);
  print_stats(game_info);
  return *game_info;
}

void init_color_pairs(void) {
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_BLACK);
}

void print_field(const GameInfo_t *field, Tetramino *current) {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      if (field->field[i][j] == 1) {
        MVADDCH(j, i * 2 + 1,
                ' ' | A_REVERSE | attron(COLOR_PAIR(current->color)));
        MVADDCH(j, i * 2 + 2,
                ' ' | A_REVERSE | attron(COLOR_PAIR(current->color)));
      } else {
        MVADDCH(j, i * 2 + 1, ' ' | A_REVERSE | attron(COLOR_PAIR(8)));
        MVADDCH(j, i * 2 + 2, ' ' | A_REVERSE | attron(COLOR_PAIR(8)));
      }
    }
  }
  attroff(COLOR_PAIR(current->color));
  attroff(COLOR_PAIR(current->color));
  attroff(COLOR_PAIR(8));
  attroff(COLOR_PAIR(8));
}

void print_stats(GameInfo_t *stats) {
  MVPRINTW(7, HEIGHT + 5, " LEVEL %d", stats->level);
  MVPRINTW(11, HEIGHT + 5, " %d", stats->score);
  MVPRINTW(15, HEIGHT + 5, " %d", stats->high_score);
  MVPRINTW(18, HEIGHT + 5, "SPEED %d", stats->speed);
}

void print_next(const GameInfo_t *next) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (next->next[j][i] == 1) {
        MVADDCH(j + 2, i * 2 + 25, ' ' | A_REVERSE | attron(COLOR_PAIR(1)));
        MVADDCH(j + 2, i * 2 + 26, ' ' | A_REVERSE | attron(COLOR_PAIR(1)));
      }
    }
  }
  attroff(COLOR_PAIR(1));
  attroff(COLOR_PAIR(1));
}