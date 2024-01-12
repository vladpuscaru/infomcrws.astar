//
// Created by Vlad Puscaru on 12.01.2024.
//

#include "Simulator.h"

Simulator::Simulator(int width, int height, const std::string &title, const std::string &fontFile)
        : m_window(sf::VideoMode(width, height), title) {
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
    m_menuView.setViewport(sf::FloatRect(0, 0.8, 1, .2));
    m_menuView.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y * .2));

    m_simView.setCenter(0, 0);
    m_simView.setViewport(sf::FloatRect(0, 0, 1, .8));
    m_menuView.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y * .8));

    sf::Font font;
    if (!font.loadFromFile(fontFile)) {
        std::cerr << "Unable to load font file " << fontFile << std::endl;
        exit(-1);
    }

    m_text.setFont(font);

    m_gridWidth = 10;
    m_gridHeight = 20;
    updateGrid();
}

void Simulator::update() {
    updateGrid();
}

void Simulator::render() {
    m_window.clear(sf::Color::Black);

//    renderSim();
//    renderMenu();

    if (!m_grid.empty()) {
        float cellWidth = m_window.getSize().x / m_gridWidth;
        float cellHeight = m_window.getSize().y / m_gridHeight;

        sf::RectangleShape rect;
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(.5f);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setSize({ cellWidth, cellHeight });
        for (int i = 0; i < m_gridHeight; i++) {
            for (int j = 0; j < m_gridWidth; j++) {
                rect.setPosition(j * cellWidth, i * cellHeight);
                m_window.draw(rect);
            }
        }
    }

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
            }
        }
    }
}

void Simulator::renderMenu() {
    m_window.setView(m_menuView);

    sf::RectangleShape rect;
    rect.setSize(m_menuView.getSize());
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Red);

    m_window.draw(rect);

    m_window.setView(m_window.getDefaultView());
}

void Simulator::renderSim() {
    m_window.setView(m_simView);

    sf::RectangleShape rect;
    rect.setSize(m_simView.getSize());
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::White);

    m_window.draw(rect);

    m_window.setView(m_window.getDefaultView());
}

void Simulator::updateGrid() {
    if (m_gridHeight != m_grid.size() || (!m_grid.empty() && m_gridWidth != m_grid[0].size())) {
        m_grid.clear();

        for (int i = 0; i < m_gridHeight; i++) {
            std::vector<int> line;
            for (int j = 0; j < m_gridWidth; j++) {
                line.push_back(0);
            }
            m_grid.push_back(line);
        }
    }
}
