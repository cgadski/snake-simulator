#include "util.h"


/**********************************************************************************************************************/
/* dir, loc
/**********************************************************************************************************************/

loc::loc(int col, int row) : x(col), y(row) { }

loc::loc(dir some_dir) {
    switch (some_dir) {
        case dir::Up: {
            y = -1, x = 0;
            break;
        }
        case dir::Down: {
            y = 1, x = 0;
            break;
        }
        case dir::Left: {
            y = 0, x = -1;
            break;
        }
        case dir::Right: {
            y = 0, x = 1;
            break;
        }
    }
}

loc::loc() : x(0), y(0) { }

void loc::operator+=(loc a_loc) {
    this->x += a_loc.x;
    this->y += a_loc.y;
}

bool operator==(loc a_loc, loc b_loc) {
    return a_loc.x == b_loc.x && a_loc.y == b_loc.y;
}

loc operator+(loc a, loc b) {
    return loc(a.x + b.x, a.y + b.y);
}

bool loc::clamp(int max_x, int max_y) {
    int nx = x;
    int ny = y;

    nx = std::min(nx, max_x), ny = std::min(ny, max_y);
    nx = std::max(nx, 0), ny = std::max(ny, 0);

    bool clamped = nx != x || ny != y;
    x = nx, y = ny;
    return clamped;
}

/**********************************************************************************************************************/
/* tickertape
/**********************************************************************************************************************/

tickertape::tickertape(const int disp_width, const int str_width)
        : disp_width(disp_width), str_width(str_width),
          loop_length(std::max(disp_width, str_width)) { }

void tickertape::tick() {
    iter = (iter + 1) % loop_length;
}

std::string tickertape::print() {
    std::string result = "";
    const int loop_len_v = loop_length;
    for (int i = 0; i < disp_width; i++) {
        result += at_index((iter + i) % loop_len_v);
    }
    return result;
}

bool ticker::tick(double delta) {
    cooldown += delta;
    active = cooldown > period;
    cooldown = std::fmod(cooldown, period);
    return active;
}

void ticker::reset() {
    cooldown = 0;
}

ticker::operator bool() { return active; }

