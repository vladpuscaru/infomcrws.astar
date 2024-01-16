//
// Created by Vlad Puscaru on 12.01.2024.
//

#ifndef ASTAR_SIMULATOR_H
#define ASTAR_SIMULATOR_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "../Algorithm/Algorithm.h"
#include "../Algorithm/AStar/AStar.h"
#include "../Algorithm/DFS/DFS.h"
#include "./Constants.h"

typedef std::vector<std::vector<int>> Grid;

class Simulator {
private:
    std::shared_ptr<Algorithm> m_algorithm;

    sf::View m_simView;
    sf::View m_menuView;

    sf::Font m_font;
    sf::Text m_text;

    sf::Vector2f m_startPos;
    sf::Vector2f m_goalPos;

    sf::RenderWindow m_window;

    std::vector<Grid> m_levelGrids;
    Grid m_grid;
    int m_gridWidth;
    int m_gridHeight;

    int m_lastActiveGrid;
    int m_activeGrid;

    void init(const std::string& fontFile);

    void update();
    void render();
    void input();

    void renderMenu();
    void renderSim();

    void updateGrid();
    void computePath();

    void resetGrid();

    void readLevelsFromFiles(const std::vector<std::string>& levelFiles);

    sf::Vector2i getCellCoordsFromWorldPos(sf::Vector2f worldPos);
public:
    Simulator(int width, int height, const std::string& title, const std::string& fontFile, const std::vector<std::string>& levelFiles);

    void run();
};


#endif //ASTAR_SIMULATOR_H
