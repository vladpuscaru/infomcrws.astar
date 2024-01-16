//
// Created by Vlad Puscaru on 16.01.2024.
//

#include "DFS.h"

DFS::DFS(const std::vector<std::vector<int>> &grid, const sf::Vector2i &start, const sf::Vector2i &goal) : Algorithm(
        grid, start, goal) {

}

std::vector<Node> DFS::computePath() {
    m_iterations.clear();

    gettimeofday(&m_startTime, nullptr);

    std::vector<Node*> closed;

    std::stack<Node*> open;
    open.push(&(m_grid[m_start.y][m_start.x]));

    Iteration iteration;
    iteration.m_open.push_back(m_grid[m_start.y][m_start.x]);

    while (!open.empty()) {
        Node* currentNode = open.top();
        open.pop();

        closed.push_back(currentNode);
        iteration.m_closed.push_back(*currentNode);


        if (currentNode->x == m_grid[m_goal.y][m_goal.x].x && currentNode->y == m_grid[m_goal.y][m_goal.x].y) {
            gettimeofday(&m_endTime, nullptr);
            m_iterations.push_back(iteration); // TODO: make all iterations, like in AStar
            return retracePath();
        }

        std::vector<Node*> neighbours = getNeighbours(*currentNode);
        for (auto & neighbour : neighbours) {
            bool isVisited = false;
            for (const auto& n : closed) {
                if (n->x == neighbour->x && n->y == neighbour->y) {
                    isVisited = true;
                    break;
                }
            }

            if (isVisited) {
                continue;
            }
            neighbour->parent = currentNode;
            open.push(neighbour);
        }
    }

    m_iterations.push_back(iteration); // TODO: make all iterations, like in AStar

    gettimeofday(&m_endTime, nullptr);

    return std::vector<Node>();
}

std::vector<Node *> DFS::getNeighbours(const Node &node) {
    std::vector<Node*> neighbours;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int x = node.x + j;
            int y = node.y + i;

            if (x == node.x && y == node.y) {
                continue;
            }

            if (x >= 0 && x < m_grid[0].size() && y >= 0 && y < m_grid.size()) {
                if (m_grid[y][x].value != CellType::OBSTACLE) {
                    neighbours.push_back(&(m_grid[y][x]));
                }
            }
        }
    }
    return neighbours;
}

std::vector<Node> DFS::retracePath() {
    std::vector<Node> path;

    const Node* current = &(m_grid[m_goal.y][m_goal.x]);
    while (current != &(m_grid[m_start.y][m_start.x])) {
        path.push_back(*current);
        current = current->parent;
    }

    return path;
}
