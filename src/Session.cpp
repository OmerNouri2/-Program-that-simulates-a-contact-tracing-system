#include <iostream>
#include "Session.h"
#include "Agent.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//constructor
Session::Session(const std::string &path): g(vector<vector<int>>()), treeType(), agents(vector<Agent*>()), infectionQueue(queue<int>()), cycleNum(0){
    //read the JSON file
    std::ifstream jsonFile(path);
    json j;
    jsonFile>>j;

    // Graph initialization
    g= Graph(retrieveGraphFromJsonAsMatrix(path));

    // TreeType initialization
    nlohmann::json treeT = j["tree"];
    if (treeT == "M") {
        treeType= MaxRank;
    }
    else if (treeT== "C"){
        treeType= Cycle;
    }
    else treeType= Root;

    // Agents initialization
    nlohmann::json agentsList = j["agents"];
    for (auto &iter:agentsList.items()) {
        if(iter.value()[0] == "V") { // if agent from type Virus
            addAgent(Virus(iter.value()[1]));
            g.occupyVirus(iter.value()[1]); // Occupy the node with the virus
        }
        else{ // agent from type ContactTracer
            addAgent(ContactTracer());
        };
    };
};

// Retrieve the graph from the Json file
std::vector<std::vector<int>> Session::retrieveGraphFromJsonAsMatrix(const std::string &path) {
    //read the JSON file
    std::ifstream jsonFile(path);
    nlohmann::json j=nlohmann::json::parse(jsonFile);
    std::vector<std::vector<int>> matrix;
    nlohmann::json graph = j["graph"];
    for (auto &iter:graph.items()) {
        std::vector<int> row;
        for (size_t i=0;i<graph.size();++i){
            row.push_back(iter.value()[i]);
        };
        matrix.push_back(row);
    };
    return matrix;
};

// Copy Constructor
Session::Session(const Session &sessionToCopy):g(sessionToCopy.g), treeType(sessionToCopy.treeType), agents(vector<Agent*>()), infectionQueue(sessionToCopy.infectionQueue), cycleNum(sessionToCopy.cycleNum) {
    for(auto& agentToAdd:sessionToCopy.agents){
        this->addAgent(*agentToAdd); // copy agent & add it to agents vector of this
    }
}

// Copy Assignment Operator
Session &Session::operator=(const Session &otherS) {
    // check for "self assignment" and do nothing in that case
    if (this == &otherS) {
        return *this;
    }
    clear(); // clear the memory until now
    // copy params:
    for (size_t i = 0; i < otherS.agents.size(); ++i) { // go through the children vector of the given reference
        addAgent(*otherS.agents[i]); // copy agent & add it to agents vector of this
    };
    treeType = otherS.treeType;
    infectionQueue = otherS.infectionQueue;
    cycleNum = otherS.cycleNum;
    return *this;
};

void Session::clear() { // delete vector - agents & its pointers
    for (auto& agent: agents) { // go through the agents vector
        if(agent != nullptr){
            delete(agent); //  delete all the agents recursively
            agent = nullptr;
        };
    };
    agents.clear(); // turn the size of the vector to be - 0
};

// Destructor
Session::~Session() {
    clear();
};

// Move Constructor
Session::Session(Session &&other):g(other.getGraph()), treeType(other.treeType), agents(vector<Agent*>()), infectionQueue(other.infectionQueue), cycleNum(other.cycleNum)  { // Move constructor
    // Check how we should copy agents vector
    for (size_t i = 0; i < other.agents.size(); ++i) { // go through the agents vector of the given reference
        this->agents.push_back(other.agents[i]->clone());
    }
    other.clear(); // delete children vector from other;
};

// Move Assignment Operator
Session &Session::operator=(Session &&other) {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    };
    this->clear(); // clear the memory until now
    for (size_t i = 0; i < other.agents.size(); ++i) { // go through the agents vector of the given reference
        this->agents.push_back(other.agents[i]->clone());
    };
    g= other.g;
    treeType = other.treeType;
    infectionQueue = other.infectionQueue; // Check this is how deep copy in queues work
    cycleNum = other.cycleNum;
    return *this;
}
void Session::simulate() {
    while (!g.isTerminate()){ // the termination conditions are not satisfied yet
        vector<Agent*> copyAgents = agents; // to make sure we dont use the new viruses in the same cycle they are created
        for(auto &agent:copyAgents){
            agent->act(*this);
        }
        cycleNum++ ;
    };
    createOutput();
};

void Session::addAgent(const Agent& agent){
    agents.push_back(agent.clone()); // add to the vector of agents, a copy of the agents
};

void Session::enqueueInfected(int nodeInd) { // nodes that are infected inserted to the 'infectionQueue'
    infectionQueue.push(nodeInd);
};
int Session::dequeueInfected() {
    if(!infectionQueue.empty()){
        int poppedNode=infectionQueue.front();
        infectionQueue.pop();
        return poppedNode;
    }
    return -1; // 'infectionQueue' is empty
};

int Session::getCycle() const {
    return cycleNum;
}

void Session::setGraph(const Graph &graph) { // setter for the graph of the session
    g= graph;
}

Graph& Session::getGraph() {
    return g;
}

TreeType Session::getTreeType() const {
    return treeType;
}

void Session::createOutput() {
    json j;
    j["graph"] = g.getEdges();
    j["infected"] = g.getInfectedNodes();
    std::ofstream output("./output.json");
    output << j;

    output.close();
}

size_t Session::getNumOfNodes() { //return number of node in graph
    return g.getEdges().size();
};
