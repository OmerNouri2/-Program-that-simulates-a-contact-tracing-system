#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include <fstream>
#include <queue>

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);
    Session(const Session &sessionToCopy); // Copy Constructor
    Session & operator=(const Session &otherS); // Copy Assignment Operator
    ~ Session(); // Destructor
    void clear();
    Session(Session&& other); // Move Constructor
    Session& operator = (Session&& other); // Move Operator
    void createOutput();
    size_t getNumOfNodes();

    static std::vector<std::vector<int>> retrieveGraphFromJsonAsMatrix(const std::string &path); // This method is static because we use it in the 'MIL' and we dont want this method will need access to 'this'
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    Graph& getGraph(); // method is not const because we want to change it, when ContactTracer act
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;
    int getCycle() const;
    
private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::queue<int> infectionQueue;
    int cycleNum;
};

#endif
