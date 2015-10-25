#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <ncurses.h>

#define PLATYPUS_SIZE 42
#define NB_PLATYPUS 20
#define NB_COLORS 7
#define TIME_TO_SLEEP 100000

// Platypus, tail to head (...*oO)
typedef struct {
	int tail;
	int dir;
	int x[PLATYPUS_SIZE];
	int y[PLATYPUS_SIZE];
} Platypus;

void platypus_init(Platypus *p, int width, int height) {
	int i,
		x = width / 2, //rand() % width,
		y = height / 2; //rand() % height;

	p->tail = 0;
	p->dir = rand() % 8;

	for (i = 0; i < PLATYPUS_SIZE; i++) {
		p->x[i] = x;
		p->y[i] = y;
	}
}

void platypus_move(Platypus *p, char *pattern) {
	int i;
	int pattern_length = strlen(pattern);

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

	for (i = 0; i < PLATYPUS_SIZE; i++) {
		// Platypus example : 012345678 (PLATYPUS_SIZE = 9)
		// Pattern example : Oo*. (pattern_length = 4)
		if (i < PLATYPUS_SIZE - pattern_length) {
			mvprintw(p->y[(i + p->tail) % PLATYPUS_SIZE], p->x[(i + p->tail) % PLATYPUS_SIZE], ".");
		} else {
			mvprintw(p->y[(i + p->tail) % PLATYPUS_SIZE], p->x[(i + p->tail) % PLATYPUS_SIZE], "%c", pattern[PLATYPUS_SIZE - i - 1]);
		}
	}
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

void print_usage() {
	printf("Usage: nudist_platypus_gold [option]\n");
	printf("\t--help\tPrint this message.\n");
	printf("\t-c\tColors, colors everywhere!\n");
}

int main(int argc, char *argv[]) {
	Platypus platypus[NB_PLATYPUS];
	int colors[NB_COLORS];
	char *pattern = "";
	time_t t;
	int color_flag = 0;
	/*int platypus_size = PLATYPUS_SIZE,
		nb_platypus = NB_PLATYPUS,
		time_to_sleep = TIME_TO_SLEEP;*/
	int i;
	
	// TODO Parse more than one argument, then more than one option in one argument
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"--help")) {
			print_usage();
			return 0;
		} else if (!strcmp(argv[i],"-c")) {
			color_flag = 1;
		}/* else if (!strcmp(argv[1],"-C")) {
			changing_color_flag = 1;
		}*/ else {
			pattern = argv[i];
		}
	}
	
	if (color_flag) {
		for (i = 0; i < NB_COLORS; i++) {
			colors[i] = i+1;
		}
	}

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int height = w.ws_row,
		width = w.ws_col;

	srand(time(&t));
	
	initscr();
	curs_set(0);
	if (color_flag) {
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
			platypus_move(&platypus[i], pattern);
			attroff(COLOR_PAIR(i % NB_COLORS + 1));
			//platypus_debug(&platypus[i]);
		}
		refresh();
		usleep(TIME_TO_SLEEP);
	}

	endwin();

	return 0;
}
