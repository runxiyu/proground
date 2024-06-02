#include <ncurses.h>

int main() {	
	char ch;

	initscr();
	cbreak(); // raw();
	noecho();
	refresh();
	ch = getch();
	printw("%c", ch);
	getch();
	endwin();

	return 0;
}
