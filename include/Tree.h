#ifndef TREE_H_
#define TREE_H_
#include "Session.h"
#include <vector>
#include <queue>

class Session;

class Tree{
public:
    Tree(int rootLabel);
    Tree(const Tree &treeToCopy); // Copy Constructor
    std::vector<Tree*> copyChild() const; // use for the copy constructor
    Tree & operator=(const Tree &aTree);
    virtual ~ Tree();
    void clear();
    Tree(Tree&& other); // Move Constructor
    Tree& operator = (Tree&& other); // Move Operator

    virtual Tree* clone() const= 0;
    void addChild(const Tree& child);
    static Tree* bfs(Session &session, int rootlable); //the BFS algorithm
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;
    int getNode() const; // if it should be private - we need this function
    std::vector<Tree*> getChildren(); //not const so we can add values in add child, but we dont want to change values after that
    std::vector<Tree*> getChildrenConst() const;


protected: // check if its Ok to change from 'private' to protected.
    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    virtual Tree* clone() const;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    virtual Tree* clone() const;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
    virtual Tree* clone() const;
};

#endif
