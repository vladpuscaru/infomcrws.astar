//
// Created by Vlad Puscaru on 12.01.2024.
//

#include "./Simulator/Simulator.h"

int main() {



    Simulator sim(1920, 1080, "A Star Simulator", "./assets/fonts/mainFont.ttf");
    sim.run();

    return 0;
}