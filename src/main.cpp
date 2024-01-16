//
// Created by Vlad Puscaru on 12.01.2024.
//

#include "./Simulator/Simulator.h"

int main() {

    /**
     * Generate random level
     */
//    for (int i = 0; i < 250; i++) {
//        for (int j = 0; j < 410; j++) {
//            int value = rand() % 11 > 8;
//            std::cout << value;
//        }
//        std::cout << std::endl;
//    }

    std::vector<std::string> levels {
      "./assets/levels/level1.txt",
      "./assets/levels/level2.txt",
      "./assets/levels/level3.txt",
      "./assets/levels/level4.txt",
      "./assets/levels/level5.txt"
    };

    Simulator sim(1920, 1080, "A Star Simulator", "./assets/fonts/mainFont.ttf", levels);
    sim.run();

    return 0;
}