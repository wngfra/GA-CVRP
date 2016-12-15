/*********************************
 *  ___      __      ___        * 
 *  \  \    /  \    /  /       *
 *   \  \  / __ \  /  /  **     *
 *    \  \/ /  \ \/  /  *  *     *
 *     \   /    \   /  * ** *   *
 *      ---      ---    SIAO     *
 * ******************************/
#ifndef _CVRP_CC_
#define _CVRP_CC_

#include "cvrp.h"
#include "gene.h"
#include "node.h"
#include "utility.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <omp.h>
#include <vector>

using namespace std;
using namespace std::chrono;

int CVRP::dimension_ = 0;

void CVRP::generateGenes() {
    // sort all customer nodes by angles to the depot
    vector<Node> nodes;
    for (int i = 1; i < dimension_; ++i) 
        nodes.push_back(Node(i + 1)); 
    
    sort(nodes.begin(), nodes.end(), [=](const Node &i, const Node &j){ return i.angle() < j.angle(); });

    for (int j = 0; j < numOfGenes_ / 2 + 1; ++j) {
        vector<Node> temp(nodes);
        // clustering by angle
        int initialAngle = (dimension_ - 1) / numOfGenes_ * j;
        rotate(temp.begin(), temp.begin() + initialAngle, temp.end());
        Gene g1(temp);
        
        random_shuffle(temp.begin(), temp.end());
        Gene g2(temp);

        genes_.push_back(g1);
        genes_.push_back(g2);
    }

    sortByCost();
    lastSolution_ = genes_[0].cost();
    
    // adding depots to the routes
    for (int k = 0; k < genes_.size(); ++k) 
        genes_[k].chop();
}

void CVRP::crossover(const double &crossoverRate) {
    vector<int> selected = selectByCost();
    
    #pragma omp parallel for
    for (int i = 0; i < selected.size() - 1; i += 2) {
        int p = selected[i];
        int q = selected[i + 1];
        
        Gene daughter(genes_[q].Rbx(genes_[p], crossoverRate));
        Gene son(genes_[p].Rbx(genes_[q], crossoverRate));
    
        if (genes_[p].cost() < genes_[q].cost()) genes_[q] = genes_[p];
        else genes_[p] = genes_[q];
        
        if (son.cost() < daughter.cost()) genes_[q] = son;
        else genes_[q] = daughter;
    }
}

vector<int> CVRP::selectByCost() {
    vector<int> index;
    vector<double> costList;
    double totalCost = 0, accumulatCost = 0;
    // calculate total cost and record every cost in the codeList 
    for (int i = 0; i < numOfGenes_; ++i) {
        costList.push_back(genes_[i].cost());
        totalCost += costList[i];
    }
    
    // use accumulatCost to calculate accumulative probability
    for (int j = numOfGenes_ - 1; j >= 0; --j) {
        accumulatCost += costList[j];

        if (generateRandom() < accumulatCost / totalCost) {
            index.push_back(j);
        }
    }
    
    return index;     
}

void CVRP::sortByCost() { sort(genes_.begin(), genes_.end(), [=](const Gene &i, const Gene &j){ return i.cost() < j.cost(); }); }

void CVRP::solve() {

    generateGenes();
    
    evolve();
    
    genes_[0].print();
}

// evolve with chopped genes
void CVRP::evolve() {
    
    double crossoverRate, mutationRate;
    double temperature = temperature_;
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    for (int i = 0; i < numOfGenerations_; ++i) {
        
        // update solutionCounter and the last solution cost
        if (lastSolution_ == genes_[0].cost()) { 
            ++solutionCounter_;
        } else {
            lastSolution_ = genes_[0].cost(); 
            solutionCounter_ = 0;
        }
        
        // adapt crossover mutation rate based on solution repetition
        crossoverRate = crossoverRate_ * exp(- 100 * solutionCounter_ / (double)numOfGenerations_);
        mutationRate = mutationRate_ + solutionCounter_ / (double)numOfGenerations_ * (1 - mutationRate_);
        temperature = temperature_ - i * temperature_ / (double)numOfGenerations_;
        
        // select and crossover
        crossover(crossoverRate);
        
        genes_[numOfGenes_ - 1] = genes_[0];
        
        #pragma omp parallel for        
        for (int m = 1; m < genes_.size(); ++m) {
            genes_[m].sequentialMutate(mutationRate, temperature);
            genes_[m].optMutation(mutationRate);
        }

        sortByCost();
        
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        
        //if (time_span.count() > 1700) break;
        /********************************
         *           debugging
         ********************************/
        printf("generation %d temperature: %.3f, cost: %.3f\n", i + 1, temperature, lastSolution_);
    }
    
}

void CVRP::setDimension(const int &dimension) { dimension_ = dimension; }

#endif 
