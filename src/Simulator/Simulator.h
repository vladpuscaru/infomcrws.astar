//
// Created by Vlad Puscaru on 12.01.2024.
//

#ifndef ASTAR_SIMULATOR_H
#define ASTAR_SIMULATOR_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> Grid;

class Simulator {
private:

    sf::View m_simView;
    sf::View m_menuView;

    sf::Text m_text;

    sf::RenderWindow m_window;

    Grid m_grid;
    int m_gridWidth;
    int m_gridHeight;

    void init(const std::string& fontFile);

    void update();
    void render();
    void input();

    void renderMenu();
    void renderSim();

    void updateGrid();
public:
    Simulator(int width, int height, const std::string& title, const std::string& fontFile);

    void run();
};


#endif //ASTAR_SIMULATOR_H
