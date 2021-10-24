#include "Tree.h"

using namespace std;

//Constructor
Tree::Tree(int rootLabel):node(rootLabel), children(vector<Tree*>()) {};
// Copy Constructor
Tree::Tree(const Tree &tree):node(tree.getNode()), children(tree.copyChild()) {};
// A function for Copy Constructor
std::vector<Tree*> Tree::copyChild() const {
    std::vector<Tree*> copiedChildren;
    for (size_t i = 0; i < getChildrenConst().size(); ++i) { // go through the original children vector
        if (this->getChildrenConst()[i] == nullptr) {
            copiedChildren[i] =  nullptr;
            copiedChildren.push_back(nullptr);
        }
        else{
             Tree* newTree = this->getChildrenConst()[i]->clone(); // use clone because we don't know which treeType
             copiedChildren.push_back(newTree);
        };
    };
    return copiedChildren;
};

void Tree::clear() { // delete vector - children & its pointers
    for (auto& child: children) { // go through the children vector
        if(child != nullptr){
            delete(child); //  delete all the children recursively
            child = nullptr;
        }
    }
    children.clear(); // turn the size of the vector to be - 0
};

// Destructors:
Tree::~Tree(){
    clear();
};

// Copy Assignment Operator
Tree & Tree::operator=(const Tree &aTree)
{
    // check for "self assignment" and do nothing in that case
    if (this == &aTree) {
        return *this;
    }
    clear(); // clear the memory until now
    children = aTree.copyChild(); // aTree.copyChild() <-- return a vector of children
    this->node = aTree.getNode();
    return *this;
};

// Move Constructor
Tree::Tree(Tree &&other):node(other.getNode()), children(other.copyChild()) {
    other.clear(); // delete children vector from other;
};

// Move Assignment Operator
Tree & Tree::operator = (Tree&& other){
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    };
    clear(); // clear the memory until now
    node = other.node;
    children = other.copyChild();
    other.clear(); // clear the children vector of other
    return *this;
};

//TreeTypes constructors
CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle)  {};
MaxRankTree::MaxRankTree(int rootLabel):Tree(rootLabel) {};
RootTree::RootTree(int rootLabel):Tree(rootLabel) {};

// Clones
Tree* CycleTree::clone() const{
    return new CycleTree(*this);
};
Tree* MaxRankTree::clone() const{
    return new MaxRankTree(*this);
};
Tree* RootTree::clone() const{
    return new RootTree(*this);
};

//the BFS method builds the tree using BFS algorithm
Tree* Tree::bfs(Session &session, int infectedNode) {
    Tree* infectedNodeTree=createTree(session, infectedNode);
    std::queue<Tree*> bfsQueue; //a queue for the bfs algorithm
    std::vector<bool>visited(session.getNumOfNodes(),false); // initial all values to false
    visited[infectedNode]=true; //map current node as visited
    bfsQueue.push(infectedNodeTree); // enqueue current node
    while (!bfsQueue.empty()) {
        Tree* popped = bfsQueue.front(); //access the first element in the queue
        bfsQueue.pop();
        std::vector<int> neighborsOfPopped = session.getGraph().getNeighbors(popped->getNode());
        for (int neighbor:neighborsOfPopped) {
            if (!visited[neighbor]){ // if this node is not already in the tree
                Tree* childTree = Tree::createTree(session, neighbor); // create tree to present the child of popped
                popped->children.push_back(childTree); // we add the children according to their appearance in 'neighborsOfPopped' //NADAV
                visited[neighbor] = true;
                bfsQueue.push(childTree); //insert the neighbor to the queue
            };
        };
    };
    return infectedNodeTree;
};

Tree* Tree::createTree(const Session& session, int rootLabel) {
    TreeType type = session.getTreeType();
    if (type == MaxRank) {
        Tree* tree = new MaxRankTree(rootLabel); //create new maxRankTree
        return tree;
    };
    if (type== Root){
        Tree* tree = new RootTree(rootLabel); // create new RootTree
        return tree;
    }
    if (type==Cycle){
        Tree* tree = new CycleTree(rootLabel,session.getCycle());// create new CycleTree
        return tree;
    };
    return nullptr;
};

std::vector<Tree*> Tree::getChildren(){
    return children;
};

std::vector<Tree*> Tree::getChildrenConst() const{
    return children;
};

int Tree::getNode() const {
    return node;
};

void Tree::addChild(const Tree &child) {
    children.push_back(child.clone());
}

int MaxRankTree::traceTree() {
    std::queue<Tree *> allNodes; //a queue for the all node in tree
    allNodes.push(this); // root is inserted at first to the queue
    int inxNodeToReturn = this->node; // // first we initialized max rank node to be the root
    size_t maxNumChild = children.size(); // first we initialized max to be the num of children of this
    while (!allNodes.empty()) { //we iterate through the tree while it still has nodes
        Tree *popped = allNodes.front(); //access the first node in the queue
        allNodes.pop(); //notice - we used BFS = that way the order of the insertion to the queue is according to the condition
        size_t numOfChild = popped->getChildren().size();
        // in case of a second tie - because 'children' vector is sorted & we used '>' we ensure both conditions
        if (numOfChild > maxNumChild) { // this condition make sure we picked the node with the smallest depth. then the left-most node in case of a tie.
            maxNumChild = numOfChild;
            inxNodeToReturn = popped->getNode(); // Change the node to be the one to return
        };
        for (Tree *child:popped->getChildren()) {
            allNodes.push(child); // insert the children to the queue
        };
    };
    return inxNodeToReturn;
};

int CycleTree::traceTree() {
    int i=0;
    Tree* parent=this;
    while (!parent->getChildren().empty() & (i<currCycle)){ //we iterate through the tree while it still has children
        parent=parent->getChildren().front();
        ++i;
    };
    return parent->getNode();
};

int RootTree::traceTree() {
  return node;
};