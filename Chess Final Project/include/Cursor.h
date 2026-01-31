#ifndef CURSOR_H
#define CURSOR_H

#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

class Cursor {
private:
    const vector<char> shapes{'+', '*', '-'};
    int shapeIndex;
    int prev_y_location;
    int y_location;
    int prev_x_location;
    int x_location;

public:
    Cursor() : y_location(0), prev_y_location(0), prev_x_location(0), x_location(0), shapeIndex(0) {}

    char show_cursor() {
        return shapes.at((shapeIndex == shapes.size()) ? (shapeIndex = 0)
            : shapeIndex++);
    }

    int get_y_location() { return y_location; }
    int get_x_location() { return x_location; }
    int get_prev_y() { return prev_y_location; }
    int get_prev_x() { return prev_x_location; }

    void set_c_location(int new_x_location, int new_y_location) {
        prev_y_location = y_location;
        prev_x_location = x_location;
        x_location = new_x_location;
        y_location = new_y_location;
    }
};

#endif