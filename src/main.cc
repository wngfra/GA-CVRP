/*********************************
 *  ___      __      ___        * 
 *  \  \    /  \    /  /       *
 *   \  \  / __ \  /  /  **     *
 *    \  \/ /  \ \/  /  *  *     *
 *     \   /    \   /  * ** *   *
 *      ---      ---   wngfra    *
 * ******************************/
#include "cvrp.h"
#include "gene.h"
#include "node.h"
#include "visualizer.h"

#include <chrono>
#include <cstdio>
#include <cstring>

using namespace std;
using namespace std::chrono;


int main(int argc, char** argv){

    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    vector<int> dimAndCap = Node::initialize(argv[1]);
    int dimension = Gene::setDimensionAndCapacity(dimAndCap);
    CVRP::setDimension(dimension);
    CVRP cvrp(120, 1000000, 0.75, 0.15, 5000);
    cvrp.solve();
   
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    printf("Time consumption: %.3f seconds.\n", time_span.count());

    // Generate visualizations
    printf("\nGenerating visualizations...\n");
    Visualizer::generateAllPlots(argv[1]);

    return 0;
}
