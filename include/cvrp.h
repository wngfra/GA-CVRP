#ifndef _CVRP_H_
#define _CVRP_H_

#include "gene.h"
#include "node.h"

#include <vector>

class CVRP {
  private:
    static int dimension_; 
    
  protected:
    int numOfGenes_, numOfGenerations_, solutionCounter_;
    double lastSolution_, crossoverRate_, mutationRate_, temperature_;
    vector<Gene> genes_;
    
  public:
      CVRP(int numOfGenes, int numOfGenerations, double crossoverRate, double mutationRate, double temperature): numOfGenes_(numOfGenes), numOfGenerations_(numOfGenerations), crossoverRate_(crossoverRate), mutationRate_(mutationRate), temperature_(temperature), solutionCounter_(0), lastSolution_(0) {};
    
    // generate genes via scanning counter-clockwise
    // routes without depots
    void generateGenes();
    
    // select and crossover
    void crossover(const double&);
   
    // evolution for chopped genes
    void evolve();
    
    // sort the genes by cost
    // increasing sequence
    void sortByCost();
    
    // automation
    void solve();
    
    // select genes by cost
    // return genes indices
    vector<int> selectByCost();
    
    static void setDimension(const int&);
};

#endif 
