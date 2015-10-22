#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include <ncurses.h>

#define PLATYPUS_SIZE 30
#define NB_PLATYPUS 100
#define NB_COLORS 7
#define TIME_TO_SLEEP 100000

typedef struct {
	int tail;
	int dir;
	int x[PLATYPUS_SIZE];
	int y[PLATYPUS_SIZE];
} Platypus;

void platypus_init(Platypus *p, int width, int height) {
	int i,
		x = width / 2,//rand() % width,
		y = height / 2;//rand() % height;

	p->tail = 0;
	p->dir = rand() % 8;

	for (i = 0; i < PLATYPUS_SIZE; i++) {
		p->x[i] = x;
		p->y[i] = y;
	}
}

void platypus_move(Platypus *p) {
	int i;

	mvprintw(p->y[p->tail], p->x[p->tail], " ");

	p->dir = (p->dir + 7 + rand() % 3) % 8; // Turn left, right or don't turn.
	switch (p->dir) {
		case 0:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] + 1;
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE];
			break;
		case 1:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] + 1;
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] - 1;
			break;
		case 2:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE];
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] - 1;
			break;
		case 3:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] - 1;
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] - 1;
			break;
		case 4:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] - 1;
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE];
			break;
		case 5:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] - 1;
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] + 1;
			break;
		case 6:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE];
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] + 1;
			break;
		case 7:
			p->x[p->tail] = p->x[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] + 1;
			p->y[p->tail] = p->y[(p->tail - 1 + PLATYPUS_SIZE) % PLATYPUS_SIZE] + 1;
			break;
	}

	p->tail = (p->tail + 1) % PLATYPUS_SIZE;

	for (i = 0; i < PLATYPUS_SIZE - 1; i++) {
		mvprintw(p->y[(i + p->tail) % PLATYPUS_SIZE], p->x[(i + p->tail) % PLATYPUS_SIZE], i == PLATYPUS_SIZE - 3 ? "*": i == PLATYPUS_SIZE - 2 ? "o" : ".");
	}
	mvprintw(p->y[(i + p->tail) % PLATYPUS_SIZE], p->x[(i + p->tail) % PLATYPUS_SIZE], "O");
}

void platypus_debug(Platypus *p) {
	int i;

	printf("Platypus:\n");
	printf(" dir: %d\n", p->dir);
	printf(" tail: %d\n", p->tail);
	printf(" x,y :");
	for (i = 0; i < PLATYPUS_SIZE; i++) {
		printf(" (%d,%d)", p->x[i], p->y[i]);
	}
	printf("\n");
}

int main(int argc, int argv[]) {
	Platypus platypus[NB_PLATYPUS];
	int colors[NB_COLORS];
	time_t t;

	int param_color = 1;

	int i = 0;
	while (i < NB_COLORS) {
		colors[i] = ++i;
	}

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int height = w.ws_row,
		width = w.ws_col;

	srand(time(&t));
	
	initscr();
	curs_set(0);
	if (param_color) {
		if(has_colors() == FALSE) {
			endwin();
			printf("Your terminal does not support color, don't use -c option.\n");
			exit(1);
		}
		start_color();
		for (i = 0; i < NB_COLORS; i++) {
			init_pair(i + 1, colors[i], COLOR_BLACK);
		}
	}

	for (i = 0; i < NB_PLATYPUS; i++) {
		platypus_init(&platypus[i], width, height);
	}

	while (1) {
		for (i = 0; i < NB_PLATYPUS; i++) {
			attron(COLOR_PAIR(i % NB_COLORS + 1));
			platypus_move(&platypus[i]);
			attroff(COLOR_PAIR(i % NB_COLORS + 1));
			//platypus_debug(&platypus[i]);
		}
		refresh();
		usleep(TIME_TO_SLEEP);
	}

	endwin();

	return 0;
}
