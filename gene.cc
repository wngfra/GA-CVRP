/*********************************
 *  ___      __      ___        * 
 *  \  \    /  \    /  /       *
 *   \  \  / __ \  /  /  **     *
 *    \  \/ /  \ \/  /  *  *     *
 *     \   /    \   /  * ** *   *
 *      ---      ---    SIAO     *
 * ******************************/
#ifndef _GENE_CC_
#define _GENE_CC_

#include "gene.h"
#include "node.h"
#include "utility.h"

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <vector>

#define MIN(a, b) ((a) <= (b)? (a):(b))
#define DEPOT Node(1)

int Gene::capacity_ = 0;
int Gene::dimension_ = 0;

Gene &Gene::operator=(const Gene &gene) {
    nodes_ = gene.nodes_;
    return *this;
}

double Gene::cost() const {
    double cost = 0;
    
    for (int i = 1; i < nodes_.size(); ++i) 
        cost += nodes_[i](nodes_[i - 1]);

    return cost;
}

// assume the gene is already chopped
void Gene::print() const {
    printf("algorithm Genetic Algorithm with specialized crossover and mutation\n");
    printf("cost %.3f\n1->", this->cost());  
    for (int i = 1; i < nodes_.size() - 1; ++i) {
        if (nodes_[i] == DEPOT) printf("1\n1->");
        else printf("%d->", nodes_[i].tag());
    }
    printf("1\n");
}

// route based crossover
Gene Gene::Rbx(const Gene &parent, const double &crossoverRate) {
    if (generateRandom() < crossoverRate) {
        vector<int> f_route{0}, m_route{0};
        
        for (int i = 1; i < nodes_.size(); ++i) {
            if (nodes_[i] == DEPOT) 
                f_route.push_back(i);
        }
        // number of routes in the current gene
        int rF = f_route.size() - 1;
        
        for (int j = 1; j < parent.nodes_.size() - 1; ++j) {
            if (parent.nodes_[j] == DEPOT) {
                m_route.push_back(j);
            }
        }
        // number of routes in the parent gene
        int rM = m_route.size() - 1;
        
        int ith = generateRandom(0, MIN(rF, rM));
        int f_start = f_route[ith], f_end = f_route[ith + 1];
        int m_start = m_route[ith], m_end = m_route[ith + 1];
        
        // pieces from parent
        vector<Node> fKid(parent.nodes_.begin() + m_start, parent.nodes_.begin() + m_end);
        // pieces from this
        vector<Node> mKid(nodes_.begin() + f_start, nodes_.begin() + f_end);
        
        for (int m = 0; m < nodes_.size(); ++m) {
            if (!contain(fKid, nodes_[m]) || nodes_[m] == DEPOT)
                fKid.push_back(nodes_[m]);
        }
        
        for (int n = 0; n < parent.nodes_.size(); ++n) {
            if (!contain(mKid, parent.nodes_[n]) || parent.nodes_[n] == DEPOT)
                mKid.push_back(parent.nodes_[n]);
        }
        
        Gene fg(fKid), mg(mKid);
        fg.validate();
        mg.validate();
        
        if (fg .cost() < mg.cost()) return fg;
        else return mg;
    } else return *this;
}

// local optimal by the way
void Gene::chop() {
    int currentLoad = capacity_;
    nodes_.insert(nodes_.begin(), DEPOT);

    // marker for the starting node of a route
    int pos(1);
    
    for (int i = 1; i < nodes_.size(); ++i) {
        if (nodes_[i].demand() <= currentLoad) 
            currentLoad -= nodes_[i].demand();
        else {
            // route ends at nodes_[i - 1]
            // sort(first, last, comp): [first=pos, last=i)
            // local optimization
            vector<Node> tmp(nodes_.begin() + pos, nodes_.begin() + i);
            sort(tmp.begin(), tmp.end(), [=](const Node &i, const Node &j){ return i(DEPOT) < j(DEPOT); });
            for (int j = 0; j < tmp.size(); ++j) {
                if (j & 1) nodes_[i - j/2 - 1] = tmp[j]; 
                else nodes_[pos + j / 2] = tmp[j];
            }

            currentLoad = capacity_ - nodes_[i].demand();
            nodes_.insert(nodes_.begin() + i, DEPOT);
            pos = ++i;
        }
    }
    nodes_.push_back(DEPOT);
}

vector<int> Gene::randomPos() {
    vector<int> pos;

    int p0(generateRandom(1, nodes_.size() - 1));
    int p1(generateRandom(p0, nodes_.size() - 1));
    pos.push_back(p0);
    pos.push_back(p1);

    return pos;
}

void Gene::sequentialMutate(const double &mutationRate, const double &temperature) {
    if (generateRandom() < mutationRate) {
        Gene gene(*this);
        vector<int> p(randomPos());
        
        for (int t = 0; t < 10; ++t) {
            // if both of the selected nodes are depot
            // do the random pick again
            while (gene.nodes_[p[0]] == gene.nodes_[p[1]]) 
                p = randomPos();
            
            switch(t % 5) {
                case 0:
                {
                    // insertion 0->1
                    gene.nodes_.insert(gene.nodes_.begin() + p[1], gene.nodes_[p[0]]);
                    gene.nodes_.erase(gene.nodes_.begin() + p[0]);
                    if (gene.validate()) {
                        this->acceptGene(gene, temperature);
                        break;
                    }
                }
                
                case 1:
                {
                    // insertion 1->0
                    gene = *this;
                    Node node = gene.nodes_[p[1]];
                    gene.nodes_.erase(gene.nodes_.begin() + p[1]);
                    gene.nodes_.insert(gene.nodes_.begin() + p[0], node);
                    if (gene.validate()) {
                        this->acceptGene(gene, temperature);
                        break;
                    }
                }
                
                case 2:
                {
                    // swap nodes
                    gene = *this;
                    swap(gene.nodes_[p[0]], gene.nodes_[p[1]]);
                    if (gene.validate()) {
                        this->acceptGene(gene, temperature);
                        break;
                    }
                }
                
                
                case 3:
                {
                    // exchange two nodes
                    gene = *this;
                    swap(gene.nodes_[p[0]], gene.nodes_[p[1]]);
                    swap(gene.nodes_[p[0] + 1], gene.nodes_[p[1] - 1]);
                    if (gene.validate()) {
                        this->acceptGene(gene, temperature);
                        break;
                    }
                }
                
                
                case 4:
                {
                    // exchange outer pieces
                    gene = *this;
                    int q;
                    for (q = p[0]; q >= 0; --q) {
                        if (gene.nodes_[q] == DEPOT) break;
                    }
                    int len = find(gene.nodes_.begin() + p[1], gene.nodes_.end(), DEPOT) - gene.nodes_.begin() - p[1];
                    int length = generateRandom( 0, MIN( p[0] - q, len ) ) - 1;
                    for (int j = 0; j < length; ++j) 
                        swap(gene.nodes_[p[0] - j], gene.nodes_[p[1] + j]);
                    if (gene.validate()) 
                        this->acceptGene(gene, temperature);
                }
            }
        }
    }
}

// if gene's cost is smaller ans it's valid, swap it with the current gene
void Gene::acceptGene(Gene &gene, const double &temperature) {
    double diffE = this->cost() - gene.cost();
    if (diffE > 0 || generateRandom() < BoltzmannProb(- diffE, temperature))
        *this = gene;
}

// work for unchopped genes
// randomly exchange nodes within every single route
void Gene::optMutation(const double &mutationRate) {
    if (generateRandom() < mutationRate) {
        // pos: record the starting position of a route
        int pos(1);
        
        for (int i = 2; i < nodes_.size(); ++i) {
            if (nodes_[i] == DEPOT) {
                int size(i - pos);
                if (size > 1) {
                    int p1, p2;
                    do {
                        p1 = generateRandom(0, size) + pos;
                        p2 = generateRandom(p1, size + pos);
                    } while (p1 == p2);
                    
                    vector<Node> org(nodes_.begin() + pos, nodes_.begin() + pos + size);
                    swap(nodes_[p1], nodes_[p2]);
                    vector<Node> curr(nodes_.begin() + pos, nodes_.begin() + pos + size);
                    if (vectorCost(org) < vectorCost(curr))
                        swap(nodes_[p1], nodes_[p2]);
                }
                
                pos = i + 1;
            }
        }
        
    }
}

// calculate the cost for one route (no depot representation)
double vectorCost(const vector<Node> &nodes) {
    double cost = nodes[0](DEPOT);
    for (int i = 1; i < nodes.size(); ++i)
        cost += nodes[i](nodes[i - 1]);
    // plus cost for one route
    return cost + nodes.back()(DEPOT);
}

// only work for chopped genes
bool Gene::validate() {
    int currentLoad = capacity_;
    vector<Node>::iterator it = nodes_.begin() + 1;
    while (it != nodes_.end()) {
        if (it->demand() <= currentLoad) {
            if (*it == DEPOT) {
                if (currentLoad < capacity_) {
                    currentLoad = capacity_;
                    it++;
                } else nodes_.erase(it - 1);
            } else {
                currentLoad -= it->demand();
                it++;
            }
        } else return false;
    }

    return true;
}

int Gene::setDimensionAndCapacity(const vector<int> &dimensionAndCapacity) {
    dimension_ = dimensionAndCapacity[0];
    capacity_ = dimensionAndCapacity[1];
    return dimension_;
}

#endif
