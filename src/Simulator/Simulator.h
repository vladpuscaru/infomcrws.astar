//
// Created by Vlad Puscaru on 12.01.2024.
//

#ifndef ASTAR_SIMULATOR_H
#define ASTAR_SIMULATOR_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> Grid;

enum CellType {
    EMPTY, START, GOAL, OPEN, CLOSED, PATH
};

class Simulator {
private:

    sf::View m_simView;
    sf::View m_menuView;

    sf::Font m_font;
    sf::Text m_text;

    sf::Vector2f m_startPos;
    sf::Vector2f m_goalPos;

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

    sf::Vector2i getCellCoordsFromWorldPos(sf::Vector2f worldPos);
public:
    Simulator(int width, int height, const std::string& title, const std::string& fontFile);

    void run();
};


#endif //ASTAR_SIMULATOR_H
