#include <check.h>
#include <ncurses.h>
#include <stdlib.h>

#include "../brick_game/tetris/backend.h"

START_TEST(test_init_game_info) {
  GameInfo_t game_info;
  init_game_info(&game_info);

  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      ck_assert_int_eq(game_info.field[i][j], 0);
    }
  }

  for (int k = 0; k < 4; k++) {
    for (int l = 0; l < 4; l++) {
      ck_assert_int_eq(game_info.next[k][l], 0);
    }
  }

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(test_get_action) {
  UserAction_t action = get_action(KEY_UP);
  UserAction_t expected_action = Up;
  ck_assert_int_eq(action, expected_action);

  action = get_action(KEY_DOWN);
  expected_action = Down;
  ck_assert_int_eq(action, expected_action);

  action = get_action(KEY_LEFT);
  expected_action = Left;
  ck_assert_int_eq(action, expected_action);

  action = get_action(KEY_RIGHT);
  expected_action = Right;
  ck_assert_int_eq(action, expected_action);

  action = get_action('p');
  expected_action = Pause;
  ck_assert_int_eq(action, expected_action);

  action = get_action(ACTION_KEY);
  expected_action = Action;
  ck_assert_int_eq(action, expected_action);

  action = get_action(ESCAPE_KEY);
  expected_action = Terminate;
  ck_assert_int_eq(action, expected_action);
}
END_TEST

START_TEST(tetris_base_test1) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);

  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Right);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Pause);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Down);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test2) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);

  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  game_info.field[0][18] = 1;
  game_info.field[1][18] = 1;
  game_info.field[2][18] = 1;
  game_info.field[3][18] = 1;
  game_info.field[4][18] = 1;

  game_info.field[5][18] = 1;
  game_info.field[6][18] = 1;
  game_info.field[7][18] = 1;
  game_info.field[8][18] = 1;
  game_info.field[9][18] = 1;
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Down);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Right);
  }
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Left);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Down);
    game_loop(&game_info, Down);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test3) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);

  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  game_info.field[0][18] = 1;
  game_info.field[1][18] = 1;
  game_info.field[2][18] = 1;
  game_info.field[3][18] = 1;
  game_info.field[4][18] = 1;

  game_info.field[5][18] = 1;
  game_info.field[6][18] = 1;
  game_info.field[7][18] = 1;
  game_info.field[8][18] = 1;
  game_info.field[9][18] = 1;
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Down);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Right);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Down);
    game_loop(&game_info, Down);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test4) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);

  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  game_info.field[0][18] = 1;
  game_info.field[1][18] = 1;
  game_info.field[2][18] = 1;
  game_info.field[3][18] = 1;
  game_info.field[4][18] = 1;

  game_info.field[5][18] = 1;
  game_info.field[6][18] = 1;
  game_info.field[7][18] = 1;
  game_info.field[8][18] = 1;
  game_info.field[9][18] = 1;
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Down);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Right);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Down);
    game_loop(&game_info, Down);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test5) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);

  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  game_info.field[0][18] = 1;
  game_info.field[1][18] = 1;
  game_info.field[2][18] = 1;
  game_info.field[3][18] = 1;
  game_info.field[4][18] = 1;

  game_info.field[5][18] = 1;
  game_info.field[6][18] = 1;
  game_info.field[7][18] = 1;
  game_info.field[8][18] = 1;
  game_info.field[9][18] = 1;
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Down);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Right);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Down);
    game_loop(&game_info, Down);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test6) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);

  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  game_info.field[0][18] = 1;
  game_info.field[1][18] = 1;
  game_info.field[2][18] = 1;
  game_info.field[3][18] = 1;
  game_info.field[4][18] = 1;

  game_info.field[5][18] = 1;
  game_info.field[6][18] = 1;
  game_info.field[7][18] = 1;
  game_info.field[8][18] = 1;
  game_info.field[9][18] = 1;
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Down);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
    game_loop(&game_info, Right);
  }

  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Down);
    game_loop(&game_info, Down);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test7) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  Tetramino *current;
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);
  current = get_current(NULL);
  current->type = Z;
  get_current(current);
  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Right);
  game_loop(&game_info, Left);
  game_loop(&game_info, Up);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);

  game_loop(&game_info, Left);
  game_loop(&game_info, Right);
  game_info.field[0][18] = 1;
  game_info.field[1][18] = 1;
  game_info.field[2][18] = 1;
  game_info.field[3][18] = 1;
  game_info.field[4][18] = 1;

  game_info.field[5][18] = 1;
  game_info.field[6][18] = 1;
  game_info.field[7][18] = 1;
  game_info.field[8][18] = 1;
  game_info.field[9][18] = 1;
  for (size_t i = 0; i < 30000; i++) {
    game_loop(&game_info, Action);
  }

  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test8) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  Tetramino current;
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);
  init_I(&current);
  next_init_I(&game_info);
  next_figure_update(&game_info, (figure)I);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  game_loop(&game_info, Action);
  for (int i = 0; i < 30000; i++) {
    figure_spawn(&game_info, I, &current);
    game_loop(&game_info, Action);
    game_loop(&game_info, Down);
    rotate(&game_info, &current);
    rotate_I(&game_info, &current);
  }

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

START_TEST(tetris_base_test9) {
  FILE *test_score = fopen("highscore_test.txt", "w");
  fclose(test_score);
  Tetramino current;
  GameInfo_t game_info;
  init_game_info(&game_info);
  game_loop(NULL, -1);
  init_Z(&current);
  next_init_Z(&game_info);
  next_figure_update(&game_info, (figure)Z);
  game_loop(&game_info, Action);
  game_loop(&game_info, Down);
  game_loop(&game_info, Action);
  figure_spawn(&game_info, Z, &current);
  for (int i = 0; i < 30000; i++) {
    if (current.type == Z) {
      rotate(&game_info, &current);
      rotate_Z(&game_info, &current);
      game_loop(&game_info, Action);
      game_loop(&game_info, Down);
      rotate(&game_info, &current);
      rotate_Z(&game_info, &current);
      game_loop(&game_info, Action);
      game_loop(&game_info, Down);
      rotate(&game_info, &current);
      rotate_Z(&game_info, &current);
      game_loop(&game_info, Action);
      game_loop(&game_info, Down);
      rotate(&game_info, &current);
      rotate_Z(&game_info, &current);
      game_loop(&game_info, Action);
      game_loop(&game_info, Down);
      rotate(&game_info, &current);
      rotate_Z(&game_info, &current);
      game_loop(&game_info, Action);
      game_loop(&game_info, Down);
      rotate(&game_info, &current);
      rotate_Z(&game_info, &current);
      game_loop(&game_info, Action);
      game_loop(&game_info, Down);
      rotate(&game_info, &current);
    }
  }
  game_loop(&game_info, Pause);
  game_loop(&game_info, Terminate);

  free_game_field(&game_info);
  free_game_next(&game_info);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_game_info);
  tcase_add_test(tc_core, test_get_action);
  tcase_add_test(tc_core, tetris_base_test1);
  tcase_add_test(tc_core, tetris_base_test2);
  tcase_add_test(tc_core, tetris_base_test3);
  tcase_add_test(tc_core, tetris_base_test4);
  tcase_add_test(tc_core, tetris_base_test5);
  tcase_add_test(tc_core, tetris_base_test6);
  tcase_add_test(tc_core, tetris_base_test7);
  tcase_add_test(tc_core, tetris_base_test8);
  tcase_add_test(tc_core, tetris_base_test9);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}