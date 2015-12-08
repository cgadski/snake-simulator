#include <thread>
#include "control.h"
#include "control_ncurses.h"

class frame_ncurses : public frame_i {
private:
    void update_dims();
public:
    frame_ncurses();

    void print_c(loc a_loc, char *) override;
    void draw_border(loc corner) override;

    void tick();
};

void frame_ncurses::update_dims() {
    int height, width;
    getmaxyx(stdscr, height, width);
    dims.y_max = height - 1;
    dims.x_max = width - 1;
}

frame_ncurses::frame_ncurses() {
    update_dims();
}

void frame_ncurses::print_c(loc a_loc, char *t) {
    move(a_loc.y, a_loc.x);
    printw(t);
}

void frame_ncurses::draw_border(loc corner) {
    int i;
    const int x = corner.x + 1;
    const int y = corner.y + 1;
    for (i = 1; i < x; ++i) {
        move(0, i), printw("=");
        move(y, i), printw("=");
    }
    for (i = 1; i < y; ++i) {
        move(i, 0), printw("|");
        move(i, x), printw("|");
    }
}

void frame_ncurses::tick() {
    update_dims();
}

/** run_game_ncurses **/

static const int delay = 10; // in milliseconds

void run_game_loop(frame_ncurses &frame, game_i &game) {
    /* TODO: fix game loop!
     * Make reported delta times more precise with something like the comments here.
     * (They are commented out because they give jittery values O__o.)
     */
    int key;
//    double delta;
//    clock_t now_stamp, then_stamp;

//    now_stamp = clock();
    while (1) {
//        then_stamp = now_stamp;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
//        now_stamp = clock();
//        delta = ((double) now_stamp - then_stamp) / 10000;

        frame.tick();
        game.tick(((double) delay) / 1000);
        game.render(frame);

        if (game.concluded) break;

        do {
            key = getch();
            game.handle(key);
        } while (key != -1);
    }
}

void run_game_ncurses(game_i &game) {
    /** initialisation code **/
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    frame_ncurses frame;

    /** game loop **/
    run_game_loop(frame, game);

    /** destruction code **/
    endwin();
}

