CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -I ./brick_game/tetris -I ./gui/cli

TESTS=./tests/tests.c
OBJ_TETRIS=out/backend.o
OBJ_CLI=out/frontend.o

INSTALL_DIR=./bin

VERSION=1.0
TARGET=tetris
DIST_DIR = $(TARGET)-$(VERSION)
DIST_SOURCE = ./Makefile ./gui ./brick_game

all: clean tetris
	./tetris

install: clean tetris
	mkdir $(INSTALL_DIR)
	cp tetris $(INSTALL_DIR)

uninstall:
	rm -fr $(INSTALL_DIR)

dist: clean
	mkdir -p $(DIST_DIR)
	zip -r $(DIST_DIR)/$(DIST_DIR).zip .

dvi: clean
	open README.md

doxy: clean
	doxygen
	open html/index.html

clean:
	rm -rf *.a *.o ./html tetris out/*.o *.html *.gcda *.gcno *.css test ./report

tetris: tetris.a $(OBJ_CLI) main.o
	$(CC) $(CFLAGS) $^ -o $@ tetris.a -lncurses

out/%.o: brick_game/tetris/%.c
	$(CC) $(CFLAGS) -c $< -o $@

out/%.o: brick_game/%.c
	$(CC) $(CFLAGS) -c $< -o $@

out/%.o: gui/cli/%.c
	$(CC) $(CFLAGS) -c $< -o $@

tetris.a: $(OBJ_TETRIS)
	ar rc $@ $(OBJ_TETRIS)
	ranlib $@

test: clean
	$(CC) $(CFLAGS) $(TESTS) -c 
	$(CC) $(CFLAGS) -o test *.o ./brick_game/tetris/*.c --coverage -lcheck -lsubunit -lm -lncurses
	chmod +x test
	./test

gcov_report: CFLAGS += -coverage
gcov_report: clean test
	gcov -f ./brick_game/tetris/backend.c
	mkdir report
	./test
	lcov --directory . --capture --output-file coverage.info
	genhtml coverage.info --output-directory ./report
	rm -f *.gcno *.gcov *.gcda
	rm -f */*.gcno */*.gcov */*.gcda
	rm -f *.info
	open report/index.html

check:
	clang-format -n ./brick_game/tetris/*.h ./brick_game/tetris/*.c ./gui/cli/*.c ./gui/cli/*.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --std=c11 ./brick_game/tetris/*.h ./brick_game/tetris/*.c ./gui/cli/*.c ./gui/cli/*.h ./unit_tests/*.h ./unit_tests/*.c ./brick_game/*.h *.c

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-report.txt ./test


.PHONY:  all install uninstall clean dvi dist tetris tetris.a test gcov_report
