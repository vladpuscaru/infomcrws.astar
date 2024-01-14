//
// Created by Vlad Puscaru on 14.01.2024.
//

#include "AStar.h"

AStar::AStar(const std::vector<std::vector<int>> &grid, const sf::Vector2i &start, const sf::Vector2i &goal)
        : Algorithm(grid, start, goal) {

}

std::vector<Node> AStar::computePath() {
    m_iterations.clear();

    gettimeofday(&m_startTime, nullptr);

    std::vector<Node*> closed;

    std::vector<Node*> open;
    open.push_back(&(m_grid[m_start.y][m_start.x]));

    while (!open.empty()) {
        Iteration iteration;
        for (auto n : open) {
            iteration.m_open.push_back(*n);
        }

        Node* currentNode = getMin(open);
        for (auto it = open.begin(); it != open.end(); it++) {
            if ((*it)->x == currentNode->x && (*it)->y == currentNode->y) {
                open.erase(it);
                break;
            }
        }
        closed.push_back(currentNode);

        for (auto n : closed) {
            iteration.m_closed.push_back(*n);
        }
        m_iterations.push_back(iteration);

        if (currentNode->x == m_grid[m_goal.y][m_goal.x].x && currentNode->y == m_grid[m_goal.y][m_goal.x].y) {
            gettimeofday(&m_endTime, nullptr);
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

            bool isOpened = false;
            for (const auto& n : open) {
                if (n->x == neighbour->x && n->y == neighbour->y) {
                    isOpened = true;
                    break;
                }
            }

            int costToNeighbour = currentNode->gCost + getDistance(*currentNode, *neighbour);
            if (costToNeighbour < neighbour->gCost || !isOpened) {
                neighbour->gCost = costToNeighbour;
                neighbour->hCost = getDistance(*neighbour, m_grid[m_goal.y][m_goal.x]);
                neighbour->parent = currentNode;

                if (!isOpened) {
                    open.push_back(neighbour);
                }
            }
        }
    }

    gettimeofday(&m_endTime, nullptr);

    return std::vector<Node>();
}

int AStar::getDistance(const Node &nodeA, const Node &nodeB) {
    int distX = std::abs(nodeA.x - nodeB.x);
    int distY = std::abs(nodeA.y - nodeB.y);

    if (distX > distY) {
        return 14 * distY + 10 * (distX - distY);
    } else {
        return 14 * distX + 10 * (distY - distX);
    }
}

std::vector<Node> AStar::retracePath() {
    std::vector<Node> path;

    const Node* current = &(m_grid[m_goal.y][m_goal.x]);
    while (current != &(m_grid[m_start.y][m_start.x])) {
        path.push_back(*current);
        current = current->parent;
    }

    return path;
}

Node* AStar::getMin(std::vector<Node*> &vec) {
    Node* minNode = vec[0];
    for (int i = 1; i < vec.size(); i++) {
        if (minNode->fCost > vec[i]->fCost || (minNode->fCost == vec[i]->fCost && minNode->hCost > vec[i]->hCost)) {
            minNode = vec[i];
        }
    }

    return minNode;
}

/**
 *
 * n n n
 * n X n
 * n n n
 *
 * @param node
 * @return
 */
std::vector<Node*> AStar::getNeighbours(const Node &node) {
    std::vector<Node*> neighbours;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int x = node.x + j;
            int y = node.y + i;

            if (x == node.x && y == node.y) {
                continue;
            }

            if (x >= 0 && x < m_grid[0].size() && y >= 0 && y < m_grid.size()) {
                neighbours.push_back(&(m_grid[y][x]));
            }
        }
    }
    return neighbours;
}


