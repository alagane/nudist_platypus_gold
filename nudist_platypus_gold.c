#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include <ncurses.h>

#define CHAR_TAB_SIZE 5
#define PRINT_TAB_SIZE 1000
#define TIME_TO_SLEEP 100000

int main(void) {
	char tab[CHAR_TAB_SIZE] = {'O', 'o', '*', '.', ' '};
	int x[PRINT_TAB_SIZE] = {1, 2, 3, 4},
		y[PRINT_TAB_SIZE] = {1, 2, 3, 4},
		step[PRINT_TAB_SIZE] = {0, -1, -2, -3};
	time_t t;

	int i, j;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int height = w.ws_row,
		width = w.ws_col;

	srand(time(&t));
	
	for (j = 0; j < 10; j++) {
		int decal = rand() % width;
		for (i = 0; i < 100; i++) {
			decal = decal + rand() % 3 - 1;
			y[j * 100 + i] = height - i - 1;
			x[j * 100 + i] = decal;
			step[j * 100 + i] = - j - i;
		}
	}

	initscr();
	curs_set(0);
	
	for (i = 0; i < PRINT_TAB_SIZE + CHAR_TAB_SIZE; i++) {
		for (j = 0; j < PRINT_TAB_SIZE; j++) {
			if (step[j] < CHAR_TAB_SIZE) {
				if (step[j] >= 0) {
					mvprintw(y[j], x[j], "%c", tab[step[j]]);
				}
				step[j]++;
			}
		}
		refresh();
		usleep(TIME_TO_SLEEP);
	}

	endwin();

	return 0;
}
