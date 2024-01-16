//
// Created by Vlad Puscaru on 16.01.2024.
//

#ifndef ASTAR_DFS_H
#define ASTAR_DFS_H

#include "../Algorithm.h"
#include <stack>


class DFS : public Algorithm {
private:
    std::vector<Node> retracePath();
    std::vector<Node*> getNeighbours(const Node &node);
public:
    DFS(const std::vector<std::vector<int>> &grid, const sf::Vector2i &start, const sf::Vector2i &goal);
    std::vector<Node> computePath() override;
};


#endif //ASTAR_DFS_H
