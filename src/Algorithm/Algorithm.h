//
// Created by Vlad Puscaru on 14.01.2024.
//

#ifndef ASTAR_ALGORITHM_H
#define ASTAR_ALGORITHM_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <sys/time.h>
#include "../Simulator/Constants.h"

struct Node {
public:
    int x;
    int y;
    const Node *parent = nullptr;

    int value;

    int gCost = 0.0f;
    int hCost = 0.0f;
    int fCost = 0.0f;

    Node(int x, int y, int value, const Node *parent = nullptr)
            : x(x), y(y), value(value), parent(parent) {

    }

    Node(const Node &that)
            : x(that.x), y(that.y), value(that.value), parent(that.parent) {

    }
};

struct Iteration {
public:
    std::vector<Node> m_open;
    std::vector<Node> m_closed;
};

class Algorithm {
protected:

    std::vector<std::vector<Node>> m_grid;
    sf::Vector2i m_start;
    sf::Vector2i m_goal;

    std::vector<Iteration> m_iterations;

    timeval m_startTime;
    timeval m_endTime;
public:
    Algorithm(const std::vector<std::vector<int>> &grid, const sf::Vector2i &start, const sf::Vector2i &goal)
            : m_start(start), m_goal(goal) {
        for (int i = 0; i < grid.size(); i++) {
            std::vector<Node> line;
            line.reserve(grid[i].size());
            for (int j = 0; j < grid[i].size(); j++) {
                line.push_back(Node(j, i, grid[i][j]));
            }
            m_grid.push_back(line);
        }
    }

    const std::vector<Iteration> &getIterations() const { return m_iterations; }

    double getLastExecutionTime() const {
        return (m_endTime.tv_usec - m_startTime.tv_usec) / 1000;
    }

    virtual std::vector<Node> computePath() = 0;
};

#endif //ASTAR_ALGORITHM_H
