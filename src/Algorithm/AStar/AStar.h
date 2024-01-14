//
// Created by Vlad Puscaru on 14.01.2024.
//

#ifndef ASTAR_ASTAR_H
#define ASTAR_ASTAR_H

#include "../Algorithm.h"
#include <cmath>
#include <queue>

class AStar : public Algorithm {
private:
    std::vector<Node> retracePath();
    int getDistance(const Node& nodeA, const Node& nodeB);

    Node* getMin(std::vector<Node*> &vec);
    std::vector<Node*> getNeighbours(const Node &node);
public:
    AStar(const std::vector<std::vector<int>> &grid, const sf::Vector2i &start, const sf::Vector2i &goal);
    std::vector<Node> computePath() override;

};


#endif //ASTAR_ASTAR_H
