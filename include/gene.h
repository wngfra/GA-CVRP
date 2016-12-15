#ifndef _GENE_H_
#define _GENE_H_

#include "node.h"

#include <algorithm>
#include <vector>

class Gene {
  private:
    static int capacity_, dimension_;
    
  protected:
    vector<Node> nodes_;
    
    
  public:
    // constructors
    Gene() {}
    Gene(const vector<Node> &nodes): nodes_(nodes) {}
    Gene(const Gene &gene): nodes_(gene.nodes_) {}
    Gene(const Gene *gp): nodes_(gp->nodes_) {}
    
    Gene &operator=(const Gene&);
    
    double cost() const;
    // if not chopped, chop it first
    void print() const;

    // crossover for chopped genes
    Gene Rbx(const Gene&, const double&);

    // insert depots to the customer node list
    void chop();
   
    vector<int> randomPos();
    // triple mutation for unchopped genes
    void sequentialMutate(const double&, const double&);
    // random pick 2 nodes to exchange
    void optMutation(const double&);
    
    // replace current gene based on some criterion
    void acceptGene(Gene&, const double&);
    // check if vehicles overloaded and remove neighboring depots
    bool validate();

    static int setDimensionAndCapacity(const vector<int>&);   
};

double vectorCost(const vector<Node>&);

#endif 
