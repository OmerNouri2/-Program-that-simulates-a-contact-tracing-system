#include "Agent.h"
Agent::Agent() {}
Agent::~Agent() {};

// Constructors
ContactTracer::ContactTracer() {};
Virus::Virus(int nodeInd):nodeInd(nodeInd){};  // Virus constructor

// Clones
Agent* ContactTracer::clone() const{
    ContactTracer* clone = new ContactTracer();
    return clone;
};
Agent* Virus::clone() const{
    //Virus* clone = new Virus(nodeInd);
    Virus* clone = new Virus(*this);
    return clone;
};

//contact tracer action:
void ContactTracer::act(Session& session) {
    int nextInfectedNode = session.dequeueInfected(); //dequeues an infected node from the infection queue
    if(nextInfectedNode == (-1))
        return;
    Tree* tree= Tree::bfs(session, nextInfectedNode); //creates a shortest path tree from the infected node using BFS algorithm
    int nodeToCut = tree->traceTree(); // Obtain an index of a node in the graph to cut
    session.getGraph().removeEdges(nodeToCut); // remove edge from the graph which are incident with this node
    delete tree;
}

//Virus action
void Virus::act(Session& session) {
    std::vector<int> neighborsOfNode = session.getGraph().getNeighbors(nodeInd); // neighborsOfNode <-- all the neighbors of this node
    if (!session.getGraph().isInfected(nodeInd)) {//if the node isn't already infected, it will now be infected
        session.getGraph().infectNode(nodeInd);
        session.enqueueInfected(nodeInd);
    };
    for (int node:neighborsOfNode) { //goes through the vector of the neighbors of the node
        if (session.getGraph().isHealthy(node)) { //if the neighbor is healthy
            session.getGraph().occupyVirus(node); //occupy the node
            session.addAgent(Virus(node)); //a new virus agent is created in the occupied node
            break;
        };
    };
};



