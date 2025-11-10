# GA-CVRP Visualization Guide

Beautiful visualization suite for the Genetic Algorithm-based Capacitated Vehicle Routing Problem solver.

## Features

This visualization suite provides:

1. **Dynamic Evolution Visualization** 📊
   - Cost convergence over generations
   - Temperature decay (simulated annealing)
   - Solution stagnation indicator
   - Per-generation improvement rate
   - Comprehensive statistics

2. **Route Visualization** 🗺️
   - 2D plot of all vehicle routes
   - Color-coded routes for each vehicle
   - Depot and customer node visualization
   - Route statistics and distribution
   - Distance and customer count analysis

## Installation

### Prerequisites

- Python 3.7 or higher
- pip package manager

### Install Dependencies

```bash
pip install -r requirements.txt
```

This will install:
- matplotlib (plotting library)
- numpy (numerical computing)
- pandas (data analysis)

## Usage

### Quick Start

Run the complete visualization suite:

```bash
python visualize_all.py
```

### Individual Visualizations

**Evolution visualization only:**
```bash
python visualize_evolution.py
```

**Route visualization only:**
```bash
python visualize_routes.py
```

### Advanced Usage

**Generate only evolution visualization:**
```bash
python visualize_all.py --evolution-only
```

**Generate only route visualization:**
```bash
python visualize_all.py --routes-only
```

**Use custom VRP and solution files:**
```bash
python visualize_all.py --vrp custom.vrp --solution custom-solution.txt
```

**Check if dependencies are installed:**
```bash
python visualize_all.py --check-deps
```

### Real-time Evolution Animation

Watch the evolution progress in real-time (while the algorithm is running):

```bash
python visualize_evolution.py --animate
```

## Workflow

### Complete Workflow

1. **Build and run the CVRP solver:**
   ```bash
   mkdir -p build && cd build
   cmake ..
   make
   ./CVRP ../fruitybun250.vrp
   cd ..
   ```

2. **Generate visualizations:**
   ```bash
   python visualize_all.py
   ```

3. **View the output images:**
   - `evolution_progress.png` - Evolution process visualization
   - `routes_visualization.png` - Final routes visualization

## Output Files

After running the solver, the following files are generated:

- `evolution_data.csv` - CSV file containing generation-by-generation evolution data
- `best-solution.txt` - Text file with the best solution found
- `evolution_progress.png` - High-resolution evolution visualization
- `routes_visualization.png` - High-resolution route visualization

## Visualization Details

### Evolution Visualization

The evolution visualization includes:

1. **Main Plot**: Cost convergence curve showing how the solution improves over generations
   - Initial and final cost annotations
   - Percentage improvement indicator

2. **Temperature Plot**: Simulated annealing temperature decay over time

3. **Stagnation Counter**: Shows periods where the solution didn't improve
   - Helps identify when the algorithm is stuck

4. **Improvement Rate**: Bar chart showing cost improvement per generation
   - Green bars: improvement
   - Red bars: no improvement or degradation

5. **Statistics Panel**: Summary of key metrics
   - Total generations
   - Initial vs final cost
   - Improvement percentage
   - Best generation
   - Statistical measures

### Route Visualization

The route visualization includes:

1. **Route Map**: 2D plot showing all vehicle routes
   - Red star: depot
   - Blue circles: customer nodes
   - Colored lines: vehicle routes
   - Arrows: route direction

2. **Route Length Chart**: Bar chart comparing the length of each route
   - Shows route balance
   - Average line for reference

3. **Customer Distribution**: Bar chart showing customers per route
   - Helps identify over/under-utilized vehicles

4. **Statistics Panel**: Comprehensive solution metrics
   - Number of routes/vehicles
   - Total and per-route statistics
   - Vehicle capacity information

## Customization

### Modifying Colors

Edit the color schemes in the Python files:

```python
colors = {
    'cost': '#2E86AB',        # Blue
    'temperature': '#A23B72',  # Purple
    'counter': '#F18F01',      # Orange
    'improvement': '#06A77D'   # Green
}
```

### Adjusting Figure Size

Change the `figsize` parameter:

```python
fig = plt.figure(figsize=(16, 10))  # (width, height) in inches
```

### Export Format

Change the output format by modifying the file extension:

```python
plt.savefig('output.pdf', dpi=300)  # PDF format
plt.savefig('output.svg', dpi=300)  # SVG format
```

## Troubleshooting

### Issue: "evolution_data.csv not found"

**Solution**: Run the CVRP solver first to generate the evolution data.

### Issue: "best-solution.txt not found"

**Solution**: Ensure the CVRP solver completed successfully and generated the solution file.

### Issue: Import errors

**Solution**: Install the required packages:
```bash
pip install -r requirements.txt
```

### Issue: Matplotlib backend errors

**Solution**: If running on a headless server, set the backend:
```python
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
```

## Performance Tips

- For large problems (>500 nodes), the visualization may take a few seconds
- Use `--evolution-only` or `--routes-only` to speed up visualization
- Reduce DPI for faster rendering: change `dpi=300` to `dpi=150` in the code

## Examples

### Example Output

After running the complete workflow, you'll get:

1. **Evolution Visualization**: Shows the genetic algorithm found a solution with ~80% improvement over 1000000 generations

2. **Route Visualization**: Shows the optimal routes for all vehicles serving all customers

## Contributing

Feel free to enhance the visualizations:
- Add more statistics
- Implement 3D visualizations
- Add interactive plots with plotly
- Create animated GIFs of the evolution process

## License

This visualization suite is part of the GA-CVRP project.
