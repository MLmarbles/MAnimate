#include "Canvas.hpp"
#include <iostream>

int main() {

    Canvas x(4, 4);

    x.setPixel(0, 0, 2);

    std::cout << x.getPixel(0, 0);
}