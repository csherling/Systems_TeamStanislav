#include <ncurses.h>

int main() {
    initscr();
    cbreak(); // pass all user characters to program (except control chars like Ctrl-Z).
    noecho(); // do not automatically echo user's keystrokes
    keypad(stdscr, TRUE); // enable arrow keys, fn keys

    mousemask(BUTTON1_CLICKED | BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    printw("Hello, world!\n");
    printw("KEY_LEFT: %d, KEY_UP: %d, KEY_DOWN: %d, KEY_RIGHT: %d\n", KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT);
    printw("KEY_MOUSE: %d\n", KEY_MOUSE);
    refresh();

    int c; // must be an int (getch() returns more data than a char)
    int x = 0;
    int y = 0;
    while (c = getch()) {
        refresh();
        if (c == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                mvprintw(event.y, event.x, "X");
                x = event.x;
                y = event.y;
            } else {
                printw("Oh noes! Mouse event was not OK!");
            }
        } else if (c == KEY_LEFT) {
            x--;
        } else if (c == KEY_RIGHT) {
            x++;
        } else if (c == KEY_UP) {
            y--;
        } else if (c == KEY_DOWN) {
            y++;
        } else {
            printw("%c", c);
            x++;
        }
        move(y, x);
    }

    endwin(); // close ncurses

    return 0;
}
