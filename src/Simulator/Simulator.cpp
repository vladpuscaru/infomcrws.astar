//
// Created by Vlad Puscaru on 12.01.2024.
//

#include "Simulator.h"

Simulator::Simulator(int width, int height, const std::string &title, const std::string &fontFile,
                     const std::vector<std::string> &levelFiles)
        : m_window(sf::VideoMode(width, height), title) {
    readLevelsFromFiles(levelFiles);
    init(fontFile);
}

void Simulator::run() {
    while (m_window.isOpen()) {
        input();
        update();
        render();
    }
}

void Simulator::init(const std::string &fontFile) {
    m_menuView.setCenter(0, 0);
    m_menuView.setSize(m_window.getSize().x, m_window.getSize().y * .2);
    m_menuView.setViewport(sf::FloatRect(0, 0.8, 1, .2));

    m_simView.setCenter(0, 0);
    m_simView.setSize(m_window.getSize().x, m_window.getSize().y * .8);
    m_simView.setViewport(sf::FloatRect(0, 0, 1, .8));

    if (!m_font.loadFromFile(fontFile)) {
        std::cerr << "Unable to load font file " << fontFile << std::endl;
        exit(-1);
    }

    m_text.setFont(m_font);

    m_startPos = {-1.f, -1.f};
    m_goalPos = {-1.f, -1.f};

    m_activeGrid = -1;
    m_lastActiveGrid = m_activeGrid - 1;

    m_gridWidth = 5;
    m_gridHeight = 5;
    updateGrid();

    m_algorithm = nullptr;
}

void Simulator::update() {
    updateGrid();
}

void Simulator::render() {
    m_window.clear(sf::Color::Black);

    renderMenu();
    renderSim();

    m_window.display();
}

void Simulator::input() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Q:
                    m_window.close();
                    break;
                case sf::Keyboard::I:
                    if (m_activeGrid == -1) {
                        m_gridWidth += 10;
                        m_gridHeight += 10;
                    }
                    break;
                case sf::Keyboard::D:
                    if (m_activeGrid == -1) {
                        if (m_gridWidth > 10) {
                            m_gridWidth -= 10;
                        }

                        if (m_gridHeight > 10) {
                            m_gridHeight -= 10;
                        }
                    }
                    break;
                case sf::Keyboard::C:
                    computePath();
                    break;
                case sf::Keyboard::Right:
                    m_activeGrid++;
                    if (m_activeGrid >= m_levelGrids.size()) {
                        m_activeGrid = -1;
                        m_gridWidth = 5;
                        m_gridHeight = 5;
                    }
                    m_startPos = { -1, -1 };
                    m_goalPos = { -1, -1 };
                    resetGrid();
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            // get the current mouse position in the window
            sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);

            // convert it to world coordinates
            sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                if (m_startPos.x != -1 && m_startPos.y != -1) {
                    sf::Vector2i cellCoords = getCellCoordsFromWorldPos(m_startPos);
                    m_grid[cellCoords.y][cellCoords.x] = CellType::EMPTY;
                }
                m_startPos = worldPos;
            } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                if (m_goalPos.x != -1 && m_goalPos.y != -1) {
                    sf::Vector2i cellCoords = getCellCoordsFromWorldPos(m_goalPos);
                    m_grid[cellCoords.y][cellCoords.x] = CellType::EMPTY;
                }
                m_goalPos = worldPos;
            }

            resetGrid();
        }
    }
}

void Simulator::renderMenu() {
    m_window.setView(m_menuView);

    float paddingH = 10;
    float paddingV = 10;

    sf::RectangleShape rect;
    rect.setSize(m_menuView.getSize());
    float x = m_menuView.getCenter().x - m_menuView.getSize().x / 2;
    float y = m_menuView.getCenter().y - m_menuView.getSize().y / 2;
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Blue);
    m_window.draw(rect);

    if (!m_grid.empty()) {
        m_text.setString("Grid size: " + std::to_string(m_gridWidth * m_gridHeight));
        m_text.setCharacterSize(16);
        m_text.setOutlineThickness(.5f);
        m_text.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);
        x = m_menuView.getCenter().x + m_menuView.getSize().x / 2 - m_text.getLocalBounds().width - paddingH;
        y = m_menuView.getCenter().y - m_menuView.getSize().y / 2 + paddingV;
        m_text.setPosition(x, y);
        m_window.draw(m_text);

        long time = 0;
        if (m_algorithm) {
            time = m_algorithm->getLastExecutionTime();
        }
        m_text.setString("Last Execution Time: " + std::to_string(time) + "ms");
        m_text.setCharacterSize(14);
        m_text.setOutlineThickness(.5f);
        m_text.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);
        x = m_menuView.getCenter().x + m_menuView.getSize().x / 2 - m_text.getLocalBounds().width - paddingH;
        y = m_menuView.getCenter().y - m_menuView.getSize().y / 2 + 16 + paddingV;
        m_text.setPosition(x, y);
        m_window.draw(m_text);

        long openCells = 0;
        if (m_algorithm) {
            openCells = m_algorithm->getLastIteration().m_open.size();
        }
        m_text.setString("Open Cells: " + std::to_string(openCells));
        m_text.setCharacterSize(14);
        m_text.setOutlineThickness(.5f);
        m_text.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);
        x = m_menuView.getCenter().x + m_menuView.getSize().x / 2 - m_text.getLocalBounds().width - paddingH;
        y = m_menuView.getCenter().y - m_menuView.getSize().y / 2 + 32 + paddingV;
        m_text.setPosition(x, y);
        m_window.draw(m_text);

        long visitedCells = 0;
        if (m_algorithm) {
            visitedCells = m_algorithm->getLastIteration().m_closed.size();
        }
        m_text.setString("Visited Cells: " + std::to_string(visitedCells));
        m_text.setCharacterSize(14);
        m_text.setOutlineThickness(.5f);
        m_text.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);
        x = m_menuView.getCenter().x + m_menuView.getSize().x / 2 - m_text.getLocalBounds().width - paddingH;
        y = m_menuView.getCenter().y - m_menuView.getSize().y / 2 + 48 + paddingV;
        m_text.setPosition(x, y);
        m_window.draw(m_text);
    }

    if (m_activeGrid == -1) {
        m_text.setString("I - [I]ncrease | D - [D]ecrease");
        m_text.setCharacterSize(14);
        m_text.setOutlineThickness(.5f);
        m_text.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);
        x = m_menuView.getCenter().x + m_menuView.getSize().x / 2 - m_text.getLocalBounds().width - paddingH;
        y = m_menuView.getCenter().y + m_menuView.getSize().y / 2 - m_text.getCharacterSize() - paddingV;
        m_text.setPosition(x, y);
        m_window.draw(m_text);
    }

    m_text.setString("Right Arrow - Change Grid");
    m_text.setCharacterSize(14);
    m_text.setOutlineThickness(.5f);
    m_text.setOutlineColor(sf::Color::White);
    m_text.setFillColor(sf::Color::White);
    x = m_menuView.getCenter().x + m_menuView.getSize().x / 2 - m_text.getLocalBounds().width - paddingH;
    y = m_menuView.getCenter().y + m_menuView.getSize().y / 2 - 2 * m_text.getCharacterSize() - paddingV;
    m_text.setPosition(x, y);
    m_window.draw(m_text);

    m_window.setView(m_window.getDefaultView());
}

void Simulator::renderSim() {
    m_window.setView(m_simView);

    if (!m_grid.empty()) {
        float cellWidth = m_simView.getSize().x / m_gridWidth;
        float cellHeight = m_simView.getSize().y / m_gridHeight;

        sf::RectangleShape rect;
        rect.setOutlineThickness(1.0f);
        rect.setOutlineColor(sf::Color::Black);
        rect.setSize({cellWidth, cellHeight});
        for (int i = 0; i < m_gridHeight; i++) {
            for (int j = 0; j < m_gridWidth; j++) {
                switch (m_grid[i][j]) {
                    case CellType::EMPTY:
                        rect.setFillColor(sf::Color::White);
                        break;
                    case CellType::OBSTACLE:
                        rect.setFillColor(sf::Color::Red);
                        break;
                    case CellType::START:
                        rect.setFillColor(sf::Color::Green);
                        break;
                    case CellType::GOAL:
                        rect.setFillColor(sf::Color::Yellow);
                        break;
                    case CellType::CLOSED:
                        rect.setFillColor(sf::Color::Blue);
                        break;
                    case CellType::OPEN:
                        rect.setFillColor(sf::Color::Cyan);
                        break;
                    case CellType::PATH:
                        rect.setFillColor(sf::Color::Yellow);
                        break;
                    default:
                        rect.setFillColor(sf::Color::Black);
                }
                float x = m_simView.getCenter().x - m_simView.getSize().x / 2 + j * cellWidth;
                float y = m_simView.getCenter().y - m_simView.getSize().y / 2 + i * cellHeight;
                rect.setPosition(x, y);
                m_window.draw(rect);
            }
        }
    }

    m_window.setView(m_window.getDefaultView());
}

void Simulator::updateGrid() {
    if (m_activeGrid == -1) {
        if (m_activeGrid != m_lastActiveGrid || (m_gridHeight != m_grid.size() || (!m_grid.empty() && m_gridWidth != m_grid[0].size()))) {
            // If grid has increased/decreased
            m_grid.clear();

            for (int i = 0; i < m_gridHeight; i++) {
                std::vector<int> line;
                for (int j = 0; j < m_gridWidth; j++) {
                    line.push_back(CellType::EMPTY);
                }
                m_grid.push_back(line);
            }
        }
    } else {
        // If active grid has changed
        if (m_activeGrid != m_lastActiveGrid) {
            m_grid = m_levelGrids[m_activeGrid];
        }
    }

    m_lastActiveGrid = m_activeGrid;


    if (m_startPos.x != -1 && m_startPos.y != -1) {
        sf::Vector2i cellCoords = getCellCoordsFromWorldPos(m_startPos);
        if (cellCoords.x != -1 && cellCoords.y != -1) {
            m_grid[cellCoords.y][cellCoords.x] = CellType::START;
        }
    }

    if (m_goalPos.x != -1 && m_goalPos.y != -1) {
        sf::Vector2i cellCoords = getCellCoordsFromWorldPos(m_goalPos);
        if (cellCoords.x != -1 && cellCoords.y != -1) {
            m_grid[cellCoords.y][cellCoords.x] = CellType::GOAL;
        }
    }


    if (!m_grid.empty()) {
        m_gridWidth = m_grid[0].size();
        m_gridHeight = m_grid.size();
    }
}

sf::Vector2i Simulator::getCellCoordsFromWorldPos(sf::Vector2f worldPos) {
    float cellWidth = m_simView.getSize().x / m_gridWidth;
    float cellHeight = m_simView.getSize().y / m_gridHeight;
    int row = worldPos.y / cellHeight;
    int col = worldPos.x / cellWidth;

    if (row >= 0 && row < m_gridHeight && col >= 0 && col < m_gridWidth) {
        return {col, row};
    } else {
        return {-1, -1};
    }
}

void Simulator::computePath() {
    sf::Vector2i start = getCellCoordsFromWorldPos(m_startPos);
    sf::Vector2i goal = getCellCoordsFromWorldPos(m_goalPos);

    if (m_startPos.x != -1 && m_startPos.y != -1 && m_goalPos.y != -1 && m_goalPos.y != -1) {
        m_algorithm = std::make_shared<AStar>(
                m_grid,
                start,
                goal
        );

        auto path = m_algorithm->computePath();
        auto iterations = m_algorithm->getIterations();
        auto &lastIteration = iterations[iterations.size() - 1];

        for (const Node &n: lastIteration.m_open) {
            m_grid[n.y][n.x] = CellType::OPEN;
        }

        for (const Node &n: lastIteration.m_closed) {
            m_grid[n.y][n.x] = CellType::CLOSED;
        }

        for (const Node &n: path) {
            m_grid[n.y][n.x] = CellType::PATH;
        }
    }
}

void Simulator::resetGrid() {
    if (m_activeGrid == -1) {
        for (int i = 0; i < m_grid.size(); i++) {
            for (int j = 0; j < m_grid[i].size(); j++) {
                m_grid[i][j] = CellType::EMPTY;
            }
        }
    } else {
        m_grid = m_levelGrids[m_activeGrid];
    }
}

void Simulator::readLevelsFromFiles(const std::vector<std::string> &levelFiles) {
    for (auto &fileName: levelFiles) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Cannot open level file " + fileName << std::endl;
            exit(-1);
        }

        Grid g;
        std::string line;
        while (std::getline(file, line)) {
            std::vector<int> row;
            for (int i = 0; i < line.length(); i++) {
                std::string value;
                value.push_back(line[i]);
                row.push_back(std::stoi(value));
            }
            g.push_back(row);
        }
        m_levelGrids.push_back(g);

        file.close();
    }
}
