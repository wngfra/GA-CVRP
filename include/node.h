#ifndef _NODE_H_
#define _NODE_H_

#include <cstdio>
#include <vector>

using namespace std;

// nodes with demands
class Node {
  private:
    static vector<int> demandList_;
    static vector< vector<double> > angleTable_, distanceTable_;
  protected:
    // the node number, starting from 1
    int tag_;
  public:
    // constructors
    Node(const int tag): tag_(tag) {}
    Node(const Node &node): tag_(node.tag_) {}
    
    // return distance between two nodes
    double operator()(const Node&) const;
    // compare operator
    bool operator<(const Node&) const;
    bool operator==(const Node&) const;
    bool operator!=(const Node&) const;

    Node &operator=(const Node &);
    
    double angle() const;
    int demand() const;
    int getCapacity() const;
    int getDimension() const;
 
    int tag() const;

    static vector<int> initialize(const char*);
};

#endif
