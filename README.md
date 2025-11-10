Hybrid Genetic Algorithm for CVRP
=================================

A Genetic Algorithm-based solver for the Capacitated Vehicle Routing Problem (CVRP) with beautiful visualizations.

## Features

- 🧬 Hybrid Genetic Algorithm with specialized crossover and mutation operators
- 🔥 Simulated Annealing for better exploration
- 📊 **Dynamic evolution process visualization**
- 🗺️ **Beautiful route visualization with statistics**
- ⚡ OpenMP parallelization for faster computation

## Requirements

- `CMake` 2.8 or later
- C++ compiler with C++11 support
- OpenMP support
- `gnuplot` (for visualization) - Install with:
  - Linux: `sudo apt-get install gnuplot`
  - macOS: `brew install gnuplot`
  - Windows: Download from http://www.gnuplot.info/

## Quick Start

### 1. Install gnuplot

```bash
# Linux
sudo apt-get install gnuplot

# macOS
brew install gnuplot
```

### 2. Build the Project

```bash
mkdir -p build && cd build
cmake ..
make
```

### 3. Run the Solver (with automatic visualization)

```bash
./CVRP ../fruitybun250.vrp
```

The solver will automatically generate beautiful visualizations after completing the optimization!

## Visualization

This project includes a **pure C++ visualization system** that creates beautiful plots using gnuplot:

1. **Evolution Process** - Shows cost convergence, temperature decay, and stagnation indicators
2. **Final Routes** - Displays the optimized vehicle routes on a 2D map with statistics

Visualizations are **automatically generated** when you run the solver. No additional steps needed!

## Output Files

- `best-solution.txt` - Best solution found by the algorithm
- `evolution_data.csv` - Generation-by-generation evolution data
- `evolution_progress.png` - Evolution visualization (after running visualization script)
- `routes_visualization.png` - Route visualization (after running visualization script)

## Algorithm Details

The solver uses:
- Route-based crossover (RBX)
- Sequential mutation with multiple operators
- Opt-mutation for local optimization
- Adaptive crossover and mutation rates
- Simulated annealing temperature schedule

## Example Problems

- `fruitybun250.vrp` - 250 customer nodes with capacity constraints

## License

See LICENSE file for details.
