#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

#include "gene.h"
#include "node.h"

#include <string>
#include <vector>

class Visualizer {
  private:
    static std::string evolution_data_file_;
    static std::string routes_data_file_;

  public:
    // Export evolution data for visualization
    static void exportEvolutionData(int generation, double best_cost,
                                    double temperature, int solution_counter);

    // Export route data for visualization
    static void exportRouteData(const Gene& best_gene);

    // Generate evolution visualization using gnuplot
    static void generateEvolutionPlot(const std::string& output_file = "evolution_progress.png");

    // Generate route visualization using gnuplot
    static void generateRoutePlot(const std::string& vrp_file,
                                  const std::string& output_file = "routes_visualization.png");

    // Generate all visualizations
    static void generateAllPlots(const std::string& vrp_file);
};

#endif
