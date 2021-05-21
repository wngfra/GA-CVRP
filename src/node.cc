/*********************************
 *  ___      __      ___        * 
 *  \  \    /  \    /  /       *
 *   \  \  / __ \  /  /  **     *
 *    \  \/ /  \ \/  /  *  *     *
 *     \   /    \   /  * ** *   *
 *      ---      ---   wngfra    *
 * ******************************/
#ifndef _NODE_CC_ 
#define _NODE_CC_

#include "node.h"
#include "utility.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

#define MIN(a, b) ((a) < (b)? (a):(b))
#define PI 3.14159

vector<int> Node::demandList_;
vector< vector<double> > Node::angleTable_, Node::distanceTable_;

double Node::operator()(const Node &node) const {
    int i = this->tag();
    int j = node.tag();
    return (i < j)? distanceTable_[i - 1][j - i]:distanceTable_[j - 1][i - j];
}

bool Node::operator<(const Node &node) const { return this->tag() < node.tag(); }

bool Node::operator==(const Node &node) const { return this->tag() == node.tag(); }

bool Node::operator!=(const Node &node) const { return !((*this) == node); }

Node& Node::operator=(const Node &node) { this->tag_ = node.tag_; return *this; }

double Node::angle() const { return angleTable_[0][this->tag() - 1]; }

int Node::demand() const { return demandList_[this->tag() - 1]; }

int Node::tag() const { return tag_; }

vector<int> Node::initialize(const char *fileName) {
    vector< vector<int> > data = readFile(fileName);
    
    vector<int> DimAndCapacity;
    int dimension = data[0][0];
    int capacity = data[0][1];
    DimAndCapacity.push_back(dimension);
    DimAndCapacity.push_back(capacity);

    // read node locations
    for (int i = 1; i < dimension + 1; ++i) {
        vector<double> distanceList, angleList;
        
        for (int j = i; j < dimension + 1; ++j) {
            int dx = data[j][0] - data[i][0];
            int dy = data[j][1] - data[i][1];
            double distance = sqrt(pow(dx, 2) + pow(dy, 2));
            double angle = arctan(dx, dy);
            distanceList.push_back(distance);
            angleList.push_back(angle);
        }
        
        distanceTable_.push_back(distanceList);
        angleTable_.push_back(angleList);
        demandList_.push_back(data[dimension + 1][i - 1]);
    }
   
    return DimAndCapacity;
}

#endif
