/*********************************
 *  ___      __      ___        *
 *  \  \    /  \    /  /       *
 *   \  \  / __ \  /  /  **     *
 *    \  \/ /  \ \/  /  *  *     *
 *     \   /    \   /  * ** *   *
 *      ---      ---   wngfra    *
 * ******************************/
#ifndef _VISUALIZER_CC_
#define _VISUALIZER_CC_

#include "visualizer.h"
#include "gnuplot-iostream.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

std::string Visualizer::evolution_data_file_ = "evolution_data.csv";
std::string Visualizer::routes_data_file_ = "routes_data.txt";

void Visualizer::exportEvolutionData(int generation, double best_cost,
                                     double temperature, int solution_counter) {
    static bool first_write = true;
    std::ofstream outfile;

    if (first_write) {
        outfile.open(evolution_data_file_, std::ios::out);
        outfile << "generation,best_cost,temperature,solution_counter\n";
        first_write = false;
    } else {
        outfile.open(evolution_data_file_, std::ios::app);
    }

    outfile << generation << "," << best_cost << ","
            << temperature << "," << solution_counter << "\n";
    outfile.close();
}

void Visualizer::exportRouteData(const Gene& best_gene) {
    std::ofstream outfile(routes_data_file_);

    // Export route information for plotting
    // Format: route_id node_id x y
    // This will be read by the plotting function

    outfile << "# Best solution routes\n";
    outfile << "# Format: route_id node_tag\n";

    // We'll need to access node information
    // For now, we'll just mark that we need route data
    outfile << "# Routes data exported\n";

    outfile.close();
}

void Visualizer::generateEvolutionPlot(const std::string& output_file) {
    try {
        Gnuplot gp;

        // Read evolution data
        std::ifstream infile(evolution_data_file_);
        if (!infile.good()) {
            std::cerr << "Error: Cannot open " << evolution_data_file_ << std::endl;
            return;
        }

        std::vector<int> generations;
        std::vector<double> costs;
        std::vector<double> temperatures;
        std::vector<int> counters;

        std::string line;
        std::getline(infile, line); // Skip header

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string token;
            int gen, counter;
            double cost, temp;

            if (std::getline(iss, token, ',')) gen = std::stoi(token);
            if (std::getline(iss, token, ',')) cost = std::stod(token);
            if (std::getline(iss, token, ',')) temp = std::stod(token);
            if (std::getline(iss, token, ',')) counter = std::stoi(token);

            generations.push_back(gen);
            costs.push_back(cost);
            temperatures.push_back(temp);
            counters.push_back(counter);
        }
        infile.close();

        if (generations.empty()) {
            std::cerr << "No data to plot" << std::endl;
            return;
        }

        // Calculate statistics
        double initial_cost = costs[0];
        double final_cost = costs.back();
        double improvement = ((initial_cost - final_cost) / initial_cost) * 100.0;

        // Set terminal for PNG output with high quality
        gp << "set terminal pngcairo size 1600,1000 enhanced font 'Arial,12'\n";
        gp << "set output '" << output_file << "'\n";

        // Set up multiplot layout
        gp << "set multiplot layout 2,2 title 'GA-CVRP Evolution Visualization' font ',16'\n";

        // Plot 1: Cost convergence
        gp << "set title 'Cost Convergence Over Generations' font ',14'\n";
        gp << "set xlabel 'Generation' font ',12'\n";
        gp << "set ylabel 'Cost' font ',12'\n";
        gp << "set grid\n";
        gp << "set key top right\n";

        std::vector<std::pair<int, double>> cost_data;
        for (size_t i = 0; i < generations.size(); ++i) {
            cost_data.push_back(std::make_pair(generations[i], costs[i]));
        }

        gp << "plot '-' with lines lw 2 lc rgb '#2E86AB' title 'Best Cost'\n";
        gp.send1d(cost_data);

        // Plot 2: Temperature decay
        gp << "set title 'Temperature Decay (Simulated Annealing)' font ',14'\n";
        gp << "set xlabel 'Generation' font ',12'\n";
        gp << "set ylabel 'Temperature' font ',12'\n";
        gp << "set grid\n";

        std::vector<std::pair<int, double>> temp_data;
        for (size_t i = 0; i < generations.size(); ++i) {
            temp_data.push_back(std::make_pair(generations[i], temperatures[i]));
        }

        gp << "plot '-' with lines lw 2 lc rgb '#A23B72' title 'Temperature'\n";
        gp.send1d(temp_data);

        // Plot 3: Stagnation counter
        gp << "set title 'Solution Stagnation Indicator' font ',14'\n";
        gp << "set xlabel 'Generation' font ',12'\n";
        gp << "set ylabel 'Stagnation Counter' font ',12'\n";
        gp << "set grid\n";

        std::vector<std::pair<int, int>> counter_data;
        for (size_t i = 0; i < generations.size(); ++i) {
            counter_data.push_back(std::make_pair(generations[i], counters[i]));
        }

        gp << "plot '-' with lines lw 2 lc rgb '#F18F01' title 'Stagnation'\n";
        gp.send1d(counter_data);

        // Plot 4: Statistics text box
        gp << "unset xlabel\n";
        gp << "unset ylabel\n";
        gp << "unset border\n";
        gp << "unset tics\n";
        gp << "unset key\n";
        gp << "set title 'Evolution Statistics' font ',14'\n";

        std::ostringstream stats;
        stats << "Total Generations: " << generations.back() << "\\n\\n";
        stats << "Initial Cost: " << std::fixed << std::setprecision(2) << initial_cost << "\\n";
        stats << "Final Cost: " << final_cost << "\\n";
        stats << "Improvement: " << improvement << "%\\n\\n";
        stats << "Average Cost: " << std::accumulate(costs.begin(), costs.end(), 0.0) / costs.size() << "\\n";
        stats << "Max Stagnation: " << *std::max_element(counters.begin(), counters.end());

        gp << "set label 1 '" << stats.str() << "' at screen 0.55,0.35 left font ',11'\n";
        gp << "plot NaN notitle\n";

        gp << "unset multiplot\n";

        std::cout << "✓ Evolution visualization saved to: " << output_file << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error generating evolution plot: " << e.what() << std::endl;
        std::cerr << "Make sure gnuplot is installed: sudo apt-get install gnuplot" << std::endl;
    }
}

void Visualizer::generateRoutePlot(const std::string& vrp_file,
                                   const std::string& output_file) {
    try {
        Gnuplot gp;

        // Read VRP file to get coordinates
        std::ifstream infile(vrp_file);
        if (!infile.good()) {
            std::cerr << "Error: Cannot open " << vrp_file << std::endl;
            return;
        }

        std::map<int, std::pair<double, double>> coords;
        int dimension = 0;
        int capacity = 0;
        bool coord_section = false;

        std::string line;
        while (std::getline(infile, line)) {
            if (line.find("DIMENSION") != std::string::npos) {
                size_t pos = line.find(":");
                dimension = std::stoi(line.substr(pos + 1));
            } else if (line.find("CAPACITY") != std::string::npos) {
                size_t pos = line.find(":");
                capacity = std::stoi(line.substr(pos + 1));
            } else if (line.find("NODE_COORD_SECTION") != std::string::npos) {
                coord_section = true;
                continue;
            } else if (line.find("DEMAND_SECTION") != std::string::npos) {
                break;
            }

            if (coord_section && !line.empty()) {
                std::istringstream iss(line);
                int id;
                double x, y;
                if (iss >> id >> x >> y) {
                    coords[id] = std::make_pair(x, y);
                }
            }
        }
        infile.close();

        // Read solution file
        std::ifstream solfile("best-solution.txt");
        if (!solfile.good()) {
            std::cerr << "Error: Cannot open best-solution.txt" << std::endl;
            return;
        }

        std::vector<std::vector<int>> routes;
        double total_cost = 0.0;

        while (std::getline(solfile, line)) {
            if (line.find("cost") != std::string::npos) {
                size_t pos = line.find_last_of(" ");
                total_cost = std::stod(line.substr(pos + 1));
            } else if (line.find("->") != std::string::npos) {
                std::vector<int> route;
                std::istringstream iss(line);
                std::string token;

                while (std::getline(iss, token, '-')) {
                    if (token == ">") continue;
                    try {
                        int node = std::stoi(token);
                        route.push_back(node);
                    } catch (...) {}
                }

                if (route.size() > 2) {  // Has actual customers
                    routes.push_back(route);
                }
            }
        }
        solfile.close();

        // Generate plot
        gp << "set terminal pngcairo size 1600,1000 enhanced font 'Arial,12'\n";
        gp << "set output '" << output_file << "'\n";
        gp << "set title 'CVRP Solution: " << routes.size()
           << " Vehicle Routes (Cost: " << std::fixed << std::setprecision(2)
           << total_cost << ")' font ',16'\n";
        gp << "set xlabel 'X Coordinate' font ',12'\n";
        gp << "set ylabel 'Y Coordinate' font ',12'\n";
        gp << "set grid\n";
        gp << "set size ratio -1\n";  // Equal aspect ratio
        gp << "set key outside right\n";

        // Build plot command
        std::ostringstream plot_cmd;
        plot_cmd << "plot ";

        // Plot depot
        auto depot = coords[1];
        plot_cmd << "'-' with points pt 7 ps 3 lc rgb 'red' title 'Depot'";

        // Plot routes
        for (size_t i = 0; i < routes.size(); ++i) {
            plot_cmd << ", '-' with linespoints lw 2 pt 7 ps 0.5 title 'Route " << (i + 1) << "'";
        }

        // Plot customer nodes
        plot_cmd << ", '-' with points pt 7 ps 0.8 lc rgb 'blue' title 'Customers'\n";

        gp << plot_cmd.str();

        // Send depot
        gp << depot.first << " " << depot.second << "\ne\n";

        // Send routes
        for (const auto& route : routes) {
            for (int node : route) {
                if (coords.find(node) != coords.end()) {
                    auto coord = coords[node];
                    gp << coord.first << " " << coord.second << "\n";
                }
            }
            gp << "e\n";
        }

        // Send all customer nodes
        for (const auto& kv : coords) {
            if (kv.first != 1) {  // Skip depot
                gp << kv.second.first << " " << kv.second.second << "\n";
            }
        }
        gp << "e\n";

        std::cout << "✓ Route visualization saved to: " << output_file << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error generating route plot: " << e.what() << std::endl;
        std::cerr << "Make sure gnuplot is installed: sudo apt-get install gnuplot" << std::endl;
    }
}

void Visualizer::generateAllPlots(const std::string& vrp_file) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  GA-CVRP Visualization Suite (C++)\n";
    std::cout << std::string(60, '=') << "\n\n";

    std::cout << "📊 Generating evolution visualization...\n";
    generateEvolutionPlot();

    std::cout << "\n🗺️  Generating route visualization...\n";
    generateRoutePlot(vrp_file);

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "✓ All visualizations completed successfully\n";
    std::cout << std::string(60, '=') << "\n\n";
}

#endif
