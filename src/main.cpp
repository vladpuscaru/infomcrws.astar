//
// Created by Vlad Puscaru on 12.01.2024.
//

#include "./Simulator/Simulator.h"

int main() {

    std::vector<std::string> levels {
      "./assets/levels/level1.txt"
    };

    Simulator sim(1920, 1080, "A Star Simulator", "./assets/fonts/mainFont.ttf", levels);
    sim.run();

    return 0;
}