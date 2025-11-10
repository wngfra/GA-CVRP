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

### For Building and Running

- `CMake` 2.8 or later
- C++ compiler with C++11 support
- OpenMP support

### For Visualization (Optional)

- Python 3.7+
- pip (for installing Python dependencies)

## Quick Start

### 1. Build the Project

```bash
mkdir -p build && cd build
cmake ..
make
```

### 2. Run the Solver

```bash
./CVRP ../fruitybun250.vrp
```

### 3. Visualize the Results

```bash
cd ..
pip install -r requirements.txt  # First time only
python visualize_all.py
```

## Visualization

This project includes a comprehensive visualization suite that creates beautiful plots for:

1. **Evolution Process** - Shows how the algorithm converges over generations
2. **Final Routes** - Displays the optimized vehicle routes on a 2D map

### Quick Visualization

After running the solver, generate all visualizations:

```bash
python visualize_all.py
```

For detailed visualization options and usage, see [VISUALIZATION.md](VISUALIZATION.md).

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
