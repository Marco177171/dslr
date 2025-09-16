// Histogram of Float data
// 0.0 |■■■■■■■■■■■■■■■■■■■■■■
// 0.1 |■■■■■■■■■■■■■■■■
// 0.2 |■■■■■
// 0.3 |■■■■■■■■■■■■■■
// 0.4 |■■■■■■■■
// 0.5 |■■■■■■■■■■■■■■■■
// 0.6 |■■■■■■■■■■
// 0.7 |■■■■■■■
// 0.8 |■■■■■■■■■■■■■■■
// 0.9 |■■■■■■■

#include "histogram.h"

void visualize() {
    initscr();
    addstr("DSRL HISTOGRAM\n");
    addstr("press any key to exit...\n");
    // refresh();
    refresh();
    getch();
    endwin();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        perror("Wrong number of arguments. 1 CSV requested. Quitting...");
        exit(1);
    }



    visualize();
    return EXIT_SUCCESS;
}