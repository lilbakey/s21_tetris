#include "backend.h"

/** @file */

UserAction_t get_action(int user_input) {
  UserAction_t action = -1;

  if (user_input == KEY_UP)
    action = Up;
  else if (user_input == KEY_DOWN)
    action = Down;
  else if (user_input == KEY_LEFT)
    action = Left;
  else if (user_input == KEY_RIGHT)
    action = Right;
  else if (user_input == 'p' || user_input == 'P')
    action = Pause;
  else if (user_input == ACTION_KEY)
    action = Action;
  else if (user_input == ESCAPE_KEY)
    action = Terminate;

  return action;
}

void init_game_info(GameInfo_t *game_info) {
  game_info->pause = 0;
  game_info->score = 0;
  game_info->speed = 1;
  game_info->level = 1;

  game_info->field = (int **)calloc(WIDTH, sizeof(int *));
  for (int i = 0; i < WIDTH; i++) {
    game_info->field[i] = (int *)calloc(HEIGHT, sizeof(int));
  }

  game_info->next = (int **)calloc(4, sizeof(int *));
  for (int j = 0; j < 4; j++) {
    game_info->next[j] = (int *)calloc(4, sizeof(int));
  }

  FILE *hi_score;
  hi_score = fopen("./hi_score.txt", "r");
  if (hi_score) {
    if (fscanf(hi_score, "%d", &game_info->high_score) == 0) {
      game_info->high_score = 0;
    }
    fclose(hi_score);
  }
}

void free_game_field(GameInfo_t *game_info) {
  if (game_info->field != NULL) {
    for (int i = 0; i < WIDTH; i++) {
      if (game_info->field[i] != NULL) {
        free(game_info->field[i]);
        game_info->field[i] = NULL;
      }
    }
    free(game_info->field);
    game_info->field = NULL;
  }
}
void free_game_next(GameInfo_t *game_info) {
  if (game_info->next != NULL) {
    for (int i = 0; i < 4; i++) {
      if (game_info->next[i] != NULL) {
        free(game_info->next[i]);
        game_info->next[i] = NULL;
      }
    }
    free(game_info->next);
    game_info->next = NULL;
  }
}

GameInfo_t *change_game_info(GameInfo_t *game_info) {
  static GameInfo_t *new_info;
  if (game_info == NULL) return new_info;
  new_info = game_info;
  return new_info;
}

GameInfo_t *get_game_info(GameInfo_t *game_info) {
  static GameInfo_t *Info;
  if (game_info == NULL) return Info;
  Info = game_info;
  return Info;
}

game_state game_loop(GameInfo_t *game_info, UserAction_t user_input) {
  static game_state state;
  static Tetramino current;
  static figure next_figure;
  static bool fall_time;
  static clock_t clock_clock;

  if (game_info == NULL) {
    state = SPAWN;
    fall_time = FALSE;
    clock_clock = clock();
    current.type = (rand() % 7) + 1;
    next_figure = (rand() % 7) + 1;
    current.x1 = 0;
    current.y1 = 0;
    current.x2 = 0;
    current.y2 = 0;
    current.x3 = 0;
    current.y3 = 0;
    current.x4 = 0;
    current.rotate_state = first_state;
    get_state(&state);
    get_current(&current);
    get_fall_time(&fall_time);
    get_next_figure(&next_figure);
  } else {
    get_game_info(game_info);
    clock_t clock_clock_2 = clock();
    if (clock_clock_2 - clock_clock >=
        1200 - ((clock_t)game_info->speed * 100 + 1000)) {
      clock_clock = clock_clock_2;
      fall_time = TRUE;
      get_fall_time(&fall_time);
    }
    userInput(user_input, 1);
    get_fall_time(&fall_time);
  }

  return state;
}

game_state *get_state(game_state *state) {
  static game_state *State;
  if (state == NULL) return State;
  State = state;
  return State;
}

Tetramino *get_current(Tetramino *current) {
  static Tetramino *Current;
  if (current == NULL) return Current;
  Current = current;
  return Current;
}

bool *get_fall_time(bool *fall_time) {
  static bool *Fall_time;
  if (fall_time == NULL) return Fall_time;
  Fall_time = fall_time;
  return Fall_time;
}

figure *get_next_figure(figure *next_figure) {
  static figure *Next_figure;
  if (next_figure == NULL) return Next_figure;
  Next_figure = next_figure;
  return Next_figure;
}

void userInput(UserAction_t action, bool hold) {
  GameInfo_t *game_info = get_game_info(NULL);
  game_state *state = get_state(NULL);
  Tetramino *current = get_current(NULL);
  const bool *fall_time = get_fall_time(NULL);
  figure *next_figure = get_next_figure(NULL);

  if (hold) {
    sigact(&action, fall_time, state, game_info, current, next_figure);
  }

  get_game_info(game_info);
  get_state(state);
  get_current(current);
  get_next_figure(next_figure);
}

void sigact(const UserAction_t *action, const bool *fall_time,
            game_state *state, GameInfo_t *game_info, Tetramino *current,
            figure *next_figure) {
  if (*action == Pause && *state == PAUSE) {
    *state = MOVING;
    game_info->pause = 0;
  } else if (*action == Pause && *state != PAUSE) {
    *state = PAUSE;
    game_info->pause = 1;
  }

  if (*fall_time && *state != GAMEOVER && *state != PAUSE && *state != START) {
    check_move_down(game_info, current, state, current->move_down);
    move_down(game_info, current);
    field_update(game_info, current);
  }

  switch (*state) {
    case SPAWN:
      on_spawn_state(state, game_info, current, next_figure);
      break;
    case MOVING:
      on_moving_state(action, game_info, current);
      break;
    case GAMEOVER:
      *state = GAMEOVER;
      break;
    default:
      break;
  }
}

void on_spawn_state(game_state *state, GameInfo_t *game_info,
                    Tetramino *current, figure *next_figure) {
  clear();
  doupdate();
  const figure *current_figure = get_next_figure(NULL);
  if (!figure_spawn(game_info, *current_figure, current)) {
    *state = GAMEOVER;
  } else {
    *next_figure = (rand() % 7) + 1;
    next_figure_update(game_info, *next_figure);
    field_update(game_info, current);
    *state = MOVING;
  }
}

void on_moving_state(const UserAction_t *action, GameInfo_t *game_info,
                     Tetramino *current) {
  switch (*action) {
    case Left:
      move_left(game_info, current);
      field_update(game_info, current);
      break;
    case Right:
      move_right(game_info, current);
      field_update(game_info, current);
      break;
    case Down:
      while (current->move_down) {
        move_down(game_info, current);
        field_update(game_info, current);
      }
      break;
    case Action:
      rotate(game_info, current);
      field_update(game_info, current);
      break;
    default:
      break;
  }
}

void rotate(GameInfo_t *game_info, Tetramino *current) {
  switch (current->type) {
    case I:
      rotate_I(game_info, current);
      break;
    case J:
      rotate_J(game_info, current);
      break;
    case L:
      rotate_L(game_info, current);
      break;
    case S:
      rotate_S(game_info, current);
      break;
    case T:
      rotate_T(game_info, current);
      break;
    case Z:
      rotate_Z(game_info, current);
      break;
    default:
      break;
  }
}

void rotate_I(GameInfo_t *game_info, Tetramino *current) {
  clear_last_figure(game_info, current);
  if (current->rotate_state == 0) {
    if (current->y1 > 1 && current->y2 > 1 && current->y3 > 1 &&
        current->y4 > 1 && current->y1 < 19 && current->y2 < 19 &&
        current->y3 < 19 && current->y4 < 19) {
      if (game_info->field[current->x1 + 2][current->y1 - 2] == 0 &&
          game_info->field[current->x2 + 1][current->y2 - 1] == 0 &&
          game_info->field[current->x3][current->y3] == 0 &&
          game_info->field[current->x4 - 1][current->y4 + 1] == 0) {
        current->x1 += 2;
        current->y1 -= 2;
        current->x2 += 1;
        current->y2 -= 1;
        current->x4 -= 1;
        current->y4 += 1;
        current->rotate_state = 1;
      }
    }
  } else {
    if (current->x1 > 1 && current->x2 > 1 && current->x3 > 1 &&
        current->x4 > 1 && current->x1 < 9 && current->x2 < 9 &&
        current->x3 < 9 && current->x4 < 9) {
      if (game_info->field[current->x1 - 2][current->y1 + 2] == 0 &&
          game_info->field[current->x2 - 1][current->y2 + 1] == 0 &&
          game_info->field[current->x3][current->y3] == 0 &&
          game_info->field[current->x4 + 1][current->y4 - 1] == 0) {
        current->x1 -= 2;
        current->y1 += 2;
        current->x2 -= 1;
        current->y2 += 1;
        current->x4 += 1;
        current->y4 -= 1;
        current->rotate_state = 0;
      }
    }
  }
}

int rotate_J_first(GameInfo_t *game_info, Tetramino *current) {
  int is_changed = 0;
  clear_last_figure(game_info, current);
  if (current->rotate_state == 0) {
    if (current->y2 < 19) {
      if (game_info->field[current->x1 + 2][current->y1] == 0 &&
          game_info->field[current->x2 + 1][current->y2 - 1] == 0 &&
          game_info->field[current->x4 - 1][current->y4 + 1] == 0) {
        current->x1 += 2;
        current->x2 += 1;
        current->y2 -= 1;
        current->x4 -= 1;
        current->y4 += 1;
        current->rotate_state = 1;
      }
    }
  } else if (current->rotate_state == 1) {
    if (current->x2 > 0) {
      if (game_info->field[current->x1][current->y1 + 2] == 0 &&
          game_info->field[current->x2 + 1][current->y2 + 1] == 0 &&
          game_info->field[current->x4 - 1][current->y4 - 1] == 0) {
        current->y1 += 2;
        current->x2 += 1;
        current->y2 += 1;
        current->x4 -= 1;
        current->y4 -= 1;
        current->rotate_state = 2;
        is_changed = 1;
      }
    }
  }
  return is_changed;
}
void rotate_J_second(const GameInfo_t *game_info, Tetramino *current) {
  if (current->rotate_state == 2) {
    if (current->y2 > 0) {
      if (game_info->field[current->x1 - 2][current->y1] == 0 &&
          game_info->field[current->x2 - 1][current->y2 + 1] == 0 &&
          game_info->field[current->x4 + 1][current->y4 - 1] == 0) {
        current->x1 -= 2;
        current->x2 -= 1;
        current->y2 += 1;
        current->x4 += 1;
        current->y4 -= 1;
        current->rotate_state = 3;
      }
    }
  } else if (current->rotate_state == 3) {
    if (current->x2 < 9) {
      if (game_info->field[current->x1][current->y1 - 2] == 0 &&
          game_info->field[current->x2 - 1][current->y2 - 1] == 0 &&
          game_info->field[current->x4 + 1][current->y4 + 1] == 0) {
        current->y1 -= 2;
        current->x2 -= 1;
        current->y2 -= 1;
        current->x4 += 1;
        current->y4 += 1;
        current->rotate_state = 0;
      }
    }
  }
}

void rotate_J(GameInfo_t *game_info, Tetramino *current) {
  if (!rotate_J_first(game_info, current)) {
    rotate_J_second(game_info, current);
  }
}

int rotate_L_first(GameInfo_t *game_info, Tetramino *current) {
  int is_changed = 0;
  clear_last_figure(game_info, current);
  if (current->rotate_state == 0) {
    if (current->y2 < 19) {
      if (game_info->field[current->x1 + 1][current->y1 - 1] == 0 &&
          game_info->field[current->x3 - 1][current->y3 + 1] == 0 &&
          game_info->field[current->x4][current->y4 + 2] == 0) {
        current->x1 += 1;
        current->y1 -= 1;
        current->x3 -= 1;
        current->y3 += 1;
        current->y4 += 2;
        current->rotate_state = 1;
      }
    }
  } else if (current->rotate_state == 1) {
    if (current->x2 > 0) {
      if (game_info->field[current->x1 + 1][current->y1 + 1] == 0 &&
          game_info->field[current->x3 - 1][current->y3 - 1] == 0 &&
          game_info->field[current->x4 - 2][current->y4] == 0) {
        current->x1 += 1;
        current->y1 += 1;
        current->x3 -= 1;
        current->y3 -= 1;
        current->x4 -= 2;
        current->rotate_state = 2;
        is_changed = 1;
      }
    }
  }
  return is_changed;
}
void rotate_L_second(const GameInfo_t *game_info, Tetramino *current) {
  if (current->rotate_state == 2) {
    if (current->y2 > 0) {
      if (game_info->field[current->x1 - 1][current->y1 + 1] == 0 &&
          game_info->field[current->x3 + 1][current->y3 - 1] == 0 &&
          game_info->field[current->x4][current->y4 - 2] == 0) {
        current->x1 -= 1;
        current->y2 += 1;
        current->x3 += 1;
        current->y3 -= 1;
        current->y4 -= 2;
        current->rotate_state = 3;
      }
    }
  } else if (current->rotate_state == 3) {
    if (current->x2 < 9) {
      if (game_info->field[current->x1 - 1][current->y1 - 1] == 0 &&
          game_info->field[current->x3 + 1][current->y3 + 1] == 0 &&
          game_info->field[current->x4 + 2][current->y4] == 0) {
        current->x1 -= 1;
        current->y2 -= 1;
        current->x3 += 1;
        current->y3 += 1;
        current->x4 += 2;
        current->rotate_state = 0;
      }
    }
  }
}

void rotate_L(GameInfo_t *game_info, Tetramino *current) {
  if (!rotate_L_first(game_info, current)) {
    rotate_L_second(game_info, current);
  }
}
void rotate_S(GameInfo_t *game_info, Tetramino *current) {
  clear_last_figure(game_info, current);
  if (current->rotate_state == 0) {
    if (current->y1 > 0) {
      if (game_info->field[current->x1 + 1][current->y1] == 0 &&
          game_info->field[current->x2][current->y2 + 1] == 0 &&
          game_info->field[current->x3 + 1][current->y3 - 2] == 0 &&
          game_info->field[current->x4][current->y4 - 1] == 0) {
        current->x1 += 1;
        current->y2 += 1;
        current->x3 += 1;
        current->y3 -= 2;
        current->y4 -= 1;
        current->rotate_state = 1;
      }
    }
  } else if (current->rotate_state == 1) {
    if (current->x3 > 0) {
      if (game_info->field[current->x1 - 1][current->y1] == 0 &&
          game_info->field[current->x2][current->y2 - 1] == 0 &&
          game_info->field[current->x3 - 1][current->y3 + 2] == 0 &&
          game_info->field[current->x4][current->y4 + 1] == 0) {
        current->x1 -= 1;
        current->y2 -= 1;
        current->x3 -= 1;
        current->y3 += 2;
        current->y4 += 1;
        current->rotate_state = 0;
      }
    }
  }
}

int rotate_T_first(GameInfo_t *game_info, Tetramino *current) {
  int is_changed = 0;
  clear_last_figure(game_info, current);
  if (current->rotate_state == 0) {
    if (current->y2 < 19) {
      if (game_info->field[current->x1 + 1][current->y1 + 1] == 0 &&
          game_info->field[current->x2 + 1][current->y2 - 1] == 0 &&
          game_info->field[current->x4 - 1][current->y4 + 1] == 0) {
        current->x1 += 1;
        current->y1 += 1;
        current->x2 += 1;
        current->y2 -= 1;
        current->x4 -= 1;
        current->y4 += 1;
        current->rotate_state = 1;
      }
    }
  } else if (current->rotate_state == 1) {
    if (current->x2 > 0) {
      if (game_info->field[current->x1 - 1][current->y1 + 1] == 0 &&
          game_info->field[current->x2 + 1][current->y2 + 1] == 0 &&
          game_info->field[current->x4 - 1][current->y4 - 1] == 0) {
        current->x1 -= 1;
        current->y1 += 1;
        current->x2 += 1;
        current->y2 += 1;
        current->x4 -= 1;
        current->y4 -= 1;
        current->rotate_state = 2;
        is_changed = 1;
      }
    }
  }
  return is_changed;
}

void rotate_T_second(const GameInfo_t *game_info, Tetramino *current) {
  if (current->rotate_state == 2) {
    if (current->y2 > 0) {
      if (game_info->field[current->x1 - 1][current->y1 - 1] == 0 &&
          game_info->field[current->x2 - 1][current->y2 + 1] == 0 &&
          game_info->field[current->x4 + 1][current->y4 - 1] == 0) {
        current->x1 -= 1;
        current->y1 -= 1;
        current->x2 -= 1;
        current->y2 += 1;
        current->x4 += 1;
        current->y4 -= 1;
        current->rotate_state = 3;
      }
    }
  } else if (current->rotate_state == 3) {
    if (current->x2 < 9) {
      if (game_info->field[current->x1 + 1][current->y1 - 1] == 0 &&
          game_info->field[current->x2 - 1][current->y2 - 1] == 0 &&
          game_info->field[current->x4 + 1][current->y4 + 1] == 0) {
        current->x1 += 1;
        current->y1 -= 1;
        current->x2 -= 1;
        current->y2 -= 1;
        current->x4 += 1;
        current->y4 += 1;
        current->rotate_state = 0;
      }
    }
  }
}

void rotate_T(GameInfo_t *game_info, Tetramino *current) {
  if (!rotate_T_first(game_info, current)) {
    rotate_T_second(game_info, current);
  }
}
void rotate_Z(GameInfo_t *game_info, Tetramino *current) {
  clear_last_figure(game_info, current);
  if (current->rotate_state == 0) {
    if (current->y1 > 0) {
      if (game_info->field[current->x1 + 2][current->y1 - 1] == 0 &&
          game_info->field[current->x2 + 1][current->y2] == 0 &&
          game_info->field[current->x3][current->y3 - 1] == 0 &&
          game_info->field[current->x4 - 1][current->y4] == 0) {
        current->x1 += 2;
        current->y1 -= 1;
        current->x2 += 1;
        current->y3 -= 1;
        current->x4 -= 1;
        current->rotate_state = 1;
      }
    }
  } else if (current->rotate_state == 1) {
    if (current->x3 > 0) {
      if (game_info->field[current->x1 - 2][current->y1 + 1] == 0 &&
          game_info->field[current->x2 - 1][current->y2] == 0 &&
          game_info->field[current->x3][current->y3 + 1] == 0 &&
          game_info->field[current->x4 + 1][current->y4] == 0) {
        current->x1 -= 2;
        current->y1 += 1;
        current->x2 -= 1;
        current->y3 += 1;
        current->x4 += 1;
        current->rotate_state = 0;
      }
    }
  }
}

void init_I(Tetramino *current) {
  current->x1 = 3;
  current->y1 = 0;
  current->x2 = 4;
  current->y2 = 0;
  current->x3 = 5;
  current->y3 = 0;
  current->x4 = 6;
  current->y4 = 0;
  current->type = I;
  current->color = COLOR_RED;
}

void init_J(Tetramino *current) {
  current->x1 = 4;
  current->y1 = 0;
  current->x2 = 4;
  current->y2 = 1;
  current->x3 = 5;
  current->y3 = 1;
  current->x4 = 6;
  current->y4 = 1;
  current->type = J;
  current->color = COLOR_GREEN;
}
void init_L(Tetramino *current) {
  current->x1 = 4;
  current->y1 = 1;
  current->x2 = 5;
  current->y2 = 1;
  current->x3 = 6;
  current->y3 = 1;
  current->x4 = 6;
  current->y4 = 0;
  current->type = L;
  current->color = COLOR_YELLOW;
}
void init_O(Tetramino *current) {
  current->x1 = 4;
  current->y1 = 0;
  current->x2 = 5;
  current->y2 = 0;
  current->x3 = 4;
  current->y3 = 1;
  current->x4 = 5;
  current->y4 = 1;
  current->type = O;
  current->color = COLOR_BLUE;
}
void init_S(Tetramino *current) {
  current->x1 = 5;
  current->y1 = 0;
  current->x2 = 6;
  current->y2 = 0;
  current->x3 = 4;
  current->y3 = 1;
  current->x4 = 5;
  current->y4 = 1;
  current->type = S;
  current->color = COLOR_MAGENTA;
}
void init_T(Tetramino *current) {
  current->x1 = 5;
  current->y1 = 0;
  current->x2 = 4;
  current->y2 = 1;
  current->x3 = 5;
  current->y3 = 1;
  current->x4 = 6;
  current->y4 = 1;
  current->type = T;
  current->color = COLOR_CYAN;
}
void init_Z(Tetramino *current) {
  current->x1 = 4;
  current->y1 = 0;
  current->x2 = 5;
  current->y2 = 0;
  current->x3 = 5;
  current->y3 = 1;
  current->x4 = 6;
  current->y4 = 1;
  current->type = Z;
  current->color = COLOR_WHITE;
}

bool figure_spawn(const GameInfo_t *game_info, figure current_type,
                  Tetramino *current) {
  switch (current_type) {
    case I:
      init_I(current);
      break;
    case J:
      init_J(current);
      break;
    case L:
      init_L(current);
      break;
    case O:
      init_O(current);
      break;
    case S:
      init_S(current);
      break;
    case T:
      init_T(current);
      break;
    case Z:
      init_Z(current);
      break;
    default:
      break;
  }
  current->rotate_state = first_state;
  current->move_down = 1;
  return is_can_spawn(game_info, current);
}

bool is_can_spawn(const GameInfo_t *game_info, const Tetramino *current) {
  bool is_spawn = TRUE;
  if (game_info->field[current->x1][current->y1] == 1 &&
      game_info->field[current->x2][current->y2] == 1 &&
      game_info->field[current->x3][current->y3] == 1 &&
      game_info->field[current->x4][current->y4] == 1) {
    is_spawn = FALSE;
  }
  return is_spawn;
}

void next_figure_update(GameInfo_t *game_info, figure next_figure) {
  switch (next_figure) {
    case I:
      next_init_I(game_info);
      break;
    case J:
      next_init_J(game_info);
      break;
    case L:
      next_init_L(game_info);
      break;
    case O:
      next_init_O(game_info);
      break;
    case S:
      next_init_S(game_info);
      break;
    case T:
      next_init_T(game_info);
      break;
    case Z:
      next_init_Z(game_info);
      break;
    default:
      break;
  }
}

void next_init_I(GameInfo_t *game_info) {
  game_info->next[1][0] = 1;
  game_info->next[1][1] = 1;
  game_info->next[1][2] = 1;
  game_info->next[1][3] = 1;

  game_info->next[2][0] = 0;
  game_info->next[2][1] = 0;
  game_info->next[2][2] = 0;
  game_info->next[2][3] = 0;
}
void next_init_J(GameInfo_t *game_info) {
  game_info->next[1][0] = 1;
  game_info->next[1][1] = 0;
  game_info->next[1][2] = 0;
  game_info->next[1][3] = 0;

  game_info->next[2][0] = 1;
  game_info->next[2][1] = 1;
  game_info->next[2][2] = 1;
  game_info->next[2][3] = 0;
}
void next_init_L(GameInfo_t *game_info) {
  game_info->next[1][0] = 0;
  game_info->next[1][1] = 0;
  game_info->next[1][2] = 1;
  game_info->next[1][3] = 0;

  game_info->next[2][0] = 1;
  game_info->next[2][1] = 1;
  game_info->next[2][2] = 1;
  game_info->next[2][3] = 0;
}
void next_init_O(GameInfo_t *game_info) {
  game_info->next[1][0] = 0;
  game_info->next[1][1] = 1;
  game_info->next[1][2] = 1;
  game_info->next[1][3] = 0;

  game_info->next[2][0] = 0;
  game_info->next[2][1] = 1;
  game_info->next[2][2] = 1;
  game_info->next[2][3] = 0;
}
void next_init_S(GameInfo_t *game_info) {
  game_info->next[1][0] = 0;
  game_info->next[1][1] = 1;
  game_info->next[1][2] = 1;
  game_info->next[1][3] = 0;

  game_info->next[2][0] = 1;
  game_info->next[2][1] = 1;
  game_info->next[2][2] = 0;
  game_info->next[2][3] = 0;
}
void next_init_T(GameInfo_t *game_info) {
  game_info->next[1][0] = 0;
  game_info->next[1][1] = 1;
  game_info->next[1][2] = 0;
  game_info->next[1][3] = 0;

  game_info->next[2][0] = 1;
  game_info->next[2][1] = 1;
  game_info->next[2][2] = 1;
  game_info->next[2][3] = 0;
}
void next_init_Z(GameInfo_t *game_info) {
  game_info->next[1][0] = 1;
  game_info->next[1][1] = 1;
  game_info->next[1][2] = 0;
  game_info->next[1][3] = 0;

  game_info->next[2][0] = 0;
  game_info->next[2][1] = 1;
  game_info->next[2][2] = 1;
  game_info->next[2][3] = 0;
}

void field_update(GameInfo_t *game_info, const Tetramino *current) {
  game_info->field[current->x1][current->y1] = 1;
  game_info->field[current->x2][current->y2] = 1;
  game_info->field[current->x3][current->y3] = 1;
  game_info->field[current->x4][current->y4] = 1;
}

void clear_last_figure(GameInfo_t *game_info, const Tetramino *current) {
  game_info->field[current->x1][current->y1] = 0;
  game_info->field[current->x2][current->y2] = 0;
  game_info->field[current->x3][current->y3] = 0;
  game_info->field[current->x4][current->y4] = 0;
}

void move_left(GameInfo_t *game_info, Tetramino *current) {
  clear_last_figure(game_info, current);
  if (current->x1 > 0 && current->x2 > 0 && current->x3 > 0 &&
      current->x4 > 0) {
    if (game_info->field[current->x1 - 1][current->y1] == 0 &&
        game_info->field[current->x2 - 1][current->y2] == 0 &&
        game_info->field[current->x3 - 1][current->y3] == 0 &&
        game_info->field[current->x4 - 1][current->y4] == 0) {
      current->x1 -= 1;
      current->x2 -= 1;
      current->x3 -= 1;
      current->x4 -= 1;
    }
  }
}

void move_right(GameInfo_t *game_info, Tetramino *current) {
  clear_last_figure(game_info, current);
  if (current->x1 < 9 && current->x2 < 9 && current->x3 < 9 &&
      current->x4 < 9) {
    if (game_info->field[current->x1 + 1][current->y1] == 0 &&
        game_info->field[current->x2 + 1][current->y2] == 0 &&
        game_info->field[current->x3 + 1][current->y3] == 0 &&
        game_info->field[current->x4 + 1][current->y4] == 0) {
      current->x1 += 1;
      current->x2 += 1;
      current->x3 += 1;
      current->x4 += 1;
    }
  }
}

void move_down(GameInfo_t *game_info, Tetramino *current) {
  clear_last_figure(game_info, current);
  if (current->y1 < 19 && current->y2 < 19 && current->y3 < 19 &&
      current->y4 < 19) {
    if (game_info->field[current->x1][current->y1 + 1] == 0 &&
        game_info->field[current->x2][current->y2 + 1] == 0 &&
        game_info->field[current->x3][current->y3 + 1] == 0 &&
        game_info->field[current->x4][current->y4 + 1] == 0) {
      current->y1 += 1;
      current->y2 += 1;
      current->y3 += 1;
      current->y4 += 1;
      current->move_down = 1;
    } else {
      current->move_down = 0;
    }
  } else {
    current->move_down = 0;
  }
}

void check_move_down(GameInfo_t *game_info, const Tetramino *current,
                     game_state *state, int move_down) {
  if (current->y1 == HEIGHT || current->y2 == HEIGHT || current->y3 == HEIGHT ||
      current->y4 == HEIGHT || !move_down) {
    figure *next_figure = get_next_figure(NULL);
    *state = SPAWN;
    get_next_figure(next_figure);
    int lines = check_lines(game_info);
    check_score(game_info, lines);
  }
}

int check_lines(GameInfo_t *game_info) {
  int lines = 0;
  for (int y = 0; y < HEIGHT; y++) {
    bool is_full = TRUE;
    for (int x = 0; x < WIDTH; x++) {
      if (game_info->field[x][y] == 0) {
        is_full = FALSE;
        break;
      }
    }
    if (is_full) {
      lines++;
      for (int y1 = y; y1 > 0; y1--) {
        for (int x1 = 0; x1 < WIDTH; x1++) {
          game_info->field[x1][y1] = game_info->field[x1][y1 - 1];
        }
      }
      for (int j = 0; j < WIDTH; j++) {
        game_info->field[0][j] = 0;
      }
    }
  }
  return lines;
}

void check_score(GameInfo_t *game_info, int lines) {
  if (lines == 1) {
    game_info->score += 100;
  }
  if (lines == 2) {
    game_info->score += 300;
  }
  if (lines == 3) {
    game_info->score += 700;
  }
  if (lines >= 4) {
    game_info->score += 1500;
  }
  if (game_info->score > game_info->high_score) {
    game_info->high_score = game_info->score;
    FILE *hi_score;
    hi_score = fopen("./hi_score.txt", "w");
    if (hi_score) {
      fprintf(hi_score, "%d", game_info->high_score);
      fclose(hi_score);
    }
  }
  if (game_info->score / 600 > 9) {
    game_info->level = 10;
  } else {
    game_info->level = game_info->score / 600 + 1;
  }
  game_info->speed = game_info->level;
}