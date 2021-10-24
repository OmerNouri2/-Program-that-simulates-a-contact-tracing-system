#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);
    enum NodeState{
        Healthy,
        Infected,
        Occupied //not sick yet
    };

    void infectNode(int nodeInd); //marks a node as infected
    bool isInfected(int nodeInd); //checks if a node is infected
    std::vector<int> getNeighbors(int nodeInd); //Gets the neighbors vector of a node
    bool isHealthy(int nodeInd); // checks if a node is healthy: not occupied or infected by the virus
    void occupyVirus(int nodeInd); //marks a node as occupied but not infected yet
    void removeEdges(int nodeToCut); // remove all edged in the Graph which are incident with this node
    bool isTerminate (); // true when each connected component of the graph is either fully infected, or doesn't contain a virus in it.
    std::vector<std::vector<int>> getEdges();
    std::vector<int> getInfectedNodes();

private:
    std::vector<std::vector<int>> edges;
    std::vector<NodeState> infection;
};

#endif
