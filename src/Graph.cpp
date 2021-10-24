#include "Graph.h"

Graph::Graph(std::vector<std::vector<int>> matrix):edges(matrix), infection(std::vector<NodeState>(matrix.size())){
    for(auto& node:infection){ // Initialize all nodes to be healthy
        node = Healthy;
    }
};

//infects a node by index
void Graph::infectNode(int nodeInd){
    infection[nodeInd] = Infected;
};

//checks if a node is infected
bool Graph::isInfected(int nodeInd){
    return infection[nodeInd]==Infected;
};

// returns a vector with the neighbors (and only the neighbors) of 'nodeInd'
std::vector<int> Graph::getNeighbors(int nodeInd) {
    std::vector<int> neighbors;
    for(size_t i=0; i<edges.size(); i++){ // i - indicate the number of node
        if(edges[nodeInd][i] == 1){
            neighbors.push_back(i);
        }
    }
    return neighbors;
};

bool Graph::isHealthy(int nodeInd) {
    return infection[nodeInd]==Healthy;
};
void Graph::occupyVirus(int nodeInd) {
    infection[nodeInd]=Occupied;
};

void Graph::removeEdges(int nodeToCut) {
    for (int node:getNeighbors(nodeToCut)){
        edges[nodeToCut][node] = 0; // go through the col of neighbors
        edges[node][nodeToCut] = 0; // go through the raw of neighbors
    };
}

bool Graph::isTerminate() {
    NodeState stateOfCurrNode; // indicate whether the node is healthy/sick
    bool stillOk = true;
    for (size_t nodeInx = 0; nodeInx < infection.size(); nodeInx++) { // go through each node in the Graph
        stateOfCurrNode = infection[nodeInx];
        if (stateOfCurrNode == Occupied)
            return false; // the program doesn't terminate when there is a node which is occupied
        std::vector<int> neighborsOfNode = getNeighbors(nodeInx);
        for (int node:neighborsOfNode) { //goes through the vector of the neighbors of the node
            NodeState stateOfCurrNeighbor = infection[node]; // set state of curr neighbor
            stillOk = (stateOfCurrNode == stateOfCurrNeighbor); // check if all neighbors with the same state as curr node
            if (!stillOk)
                return stillOk; // which is now false, no need to check the rest
        };
    };
    return stillOk; // which should be true - means need to terminate the algo
};


std::vector<std::vector<int>> Graph::getEdges() {
    return edges;
}

std::vector<int> Graph::getInfectedNodes() {
    std::vector<int> infectedToReturn;
    for(size_t i = 0; i<infection.size(); i++){
        if(infection[i] == Infected)
            infectedToReturn.push_back(i);
    }
    return infectedToReturn;
};
