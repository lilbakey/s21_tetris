#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"

/** @file */

/*!
  \brief Игра тетрис

  \param void
 */
void tetris_game(void);

/*!
  \brief Функция инициализации ncurses

  \param void
 */
void init_ncurses(void);

/*!
  \brief Функция старта программы

  \param void
 */
int main(void) {
  init_ncurses();
  int game = 0;
  while (TRUE) {
    select_game(game);
    UserAction_t action = get_action(getch());
    if (action == Up && game < 1) {
      game = 2;
    } else if (action == Down && game > 1) {
      game = 0;
    } else if (action == Down) {
      game++;
    } else if (action == Up) {
      game--;
    } else if (action == Action) {
      break;
    }
    select_game(game);
  }
  clear();
  print_overlay();
  UserAction_t action = Start;
  bool break_flag = TRUE;

  while (break_flag) {
    action = get_action(getch());
    if (action == Terminate) {
      action = Terminate;
      break_flag = FALSE;
    }
    if (action == Action) {
      action = Start;
      break_flag = FALSE;
    }
  }

  if (game == 0 && action == Start) {
    tetris_game();
  }

  endwin();
  return 0;
}

void init_ncurses(void) {
  initscr();
  start_color();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  srand((unsigned int)time(NULL));
  init_color_pairs();
}

void tetris_game(void) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  change_game_info(&game_info);
  game_state state = -1;
  UserAction_t action = -1;
  game_loop(NULL, action);

  while (state != GAMEOVER && state != EXIT_STATE && action != Terminate) {
    if (state == START || state == MOVING || state == PAUSE) {
      action = get_action(getch());
    }
    timeout(15000 / (game_info.speed * 100));
    state = game_loop(&game_info, action);
    updateCurrentState();
    print_overlay();
  }
  clear();
  if (state == GAMEOVER) {
    print_gameover();
    while (TRUE) {
      action = get_action(getch());
      if (action == Action) {
        clear();
        print_overlay();
        tetris_game();
      } else if (action == Terminate) {
        clear();
        break;
      }
    }
  } else if (state == EXIT_STATE) {
    clear();
  }

  free_game_field(&game_info);
  free_game_next(&game_info);
}
