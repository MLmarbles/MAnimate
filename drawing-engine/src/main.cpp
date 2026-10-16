#include "Canvas.hpp"
#include <iostream>
#include <Color.hpp>

int main() {

    Color color{255, 128, 64, 255};

    std::cout << std::hex << color.pack() << '\n';

    Color yeet = Color::unpack(0b11111111111111111111111111111111);

    std::cout << static_cast<int>(yeet.r);
}