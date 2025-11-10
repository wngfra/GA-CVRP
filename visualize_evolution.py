#!/usr/bin/env python3
"""
Visualization script for CVRP Genetic Algorithm evolution process.
Creates beautiful plots showing convergence, temperature decay, and stagnation.
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib.animation import FuncAnimation
import numpy as np
import sys
import os

def create_evolution_plots(csv_file='evolution_data.csv', output_file='evolution_progress.png', animate=False):
    """
    Create visualization of the genetic algorithm evolution process.

    Args:
        csv_file: Path to the evolution data CSV file
        output_file: Path to save the output image
        animate: If True, create an animated plot (for real-time visualization)
    """

    if not os.path.exists(csv_file):
        print(f"Error: {csv_file} not found!")
        return

    # Read evolution data
    data = pd.read_csv(csv_file)

    # Create figure with custom style
    plt.style.use('seaborn-v0_8-darkgrid' if 'seaborn-v0_8-darkgrid' in plt.style.available else 'default')
    fig = plt.figure(figsize=(16, 10))
    gs = gridspec.GridSpec(3, 2, figure=fig, hspace=0.3, wspace=0.3)

    # Color scheme
    colors = {
        'cost': '#2E86AB',
        'temperature': '#A23B72',
        'counter': '#F18F01',
        'improvement': '#06A77D'
    }

    # 1. Main plot: Cost convergence (spans 2 columns)
    ax1 = fig.add_subplot(gs[0, :])
    ax1.plot(data['generation'], data['best_cost'],
             linewidth=2.5, color=colors['cost'], alpha=0.8, label='Best Cost')
    ax1.fill_between(data['generation'], data['best_cost'].min(), data['best_cost'],
                      alpha=0.2, color=colors['cost'])
    ax1.set_xlabel('Generation', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Cost', fontsize=14, fontweight='bold')
    ax1.set_title('GA-CVRP: Cost Convergence Over Generations',
                  fontsize=16, fontweight='bold', pad=20)
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.legend(fontsize=12, loc='upper right')

    # Add initial and final cost annotations
    initial_cost = data['best_cost'].iloc[0]
    final_cost = data['best_cost'].iloc[-1]
    improvement = ((initial_cost - final_cost) / initial_cost) * 100

    ax1.annotate(f'Initial: {initial_cost:.2f}',
                xy=(data['generation'].iloc[0], initial_cost),
                xytext=(10, 20), textcoords='offset points',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='yellow', alpha=0.5),
                fontsize=10, fontweight='bold')

    ax1.annotate(f'Final: {final_cost:.2f}\n↓ {improvement:.2f}%',
                xy=(data['generation'].iloc[-1], final_cost),
                xytext=(-100, 20), textcoords='offset points',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='lightgreen', alpha=0.7),
                fontsize=10, fontweight='bold',
                arrowprops=dict(arrowstyle='->', color='green', lw=2))

    # 2. Temperature decay
    ax2 = fig.add_subplot(gs[1, 0])
    ax2.plot(data['generation'], data['temperature'],
             linewidth=2, color=colors['temperature'], marker='o',
             markersize=2, alpha=0.7)
    ax2.set_xlabel('Generation', fontsize=12, fontweight='bold')
    ax2.set_ylabel('Temperature', fontsize=12, fontweight='bold')
    ax2.set_title('Simulated Annealing: Temperature Decay',
                  fontsize=13, fontweight='bold')
    ax2.grid(True, alpha=0.3, linestyle='--')
    ax2.fill_between(data['generation'], 0, data['temperature'],
                      alpha=0.2, color=colors['temperature'])

    # 3. Solution counter (stagnation indicator)
    ax3 = fig.add_subplot(gs[1, 1])
    ax3.plot(data['generation'], data['solution_counter'],
             linewidth=2, color=colors['counter'], alpha=0.7)
    ax3.fill_between(data['generation'], 0, data['solution_counter'],
                      alpha=0.3, color=colors['counter'])
    ax3.set_xlabel('Generation', fontsize=12, fontweight='bold')
    ax3.set_ylabel('Stagnation Counter', fontsize=12, fontweight='bold')
    ax3.set_title('Solution Stagnation Indicator',
                  fontsize=13, fontweight='bold')
    ax3.grid(True, alpha=0.3, linestyle='--')

    # 4. Cost improvement rate (first derivative)
    ax4 = fig.add_subplot(gs[2, 0])
    cost_diff = -data['best_cost'].diff().fillna(0)
    ax4.bar(data['generation'], cost_diff,
            color=[colors['improvement'] if x > 0 else 'lightcoral' for x in cost_diff],
            alpha=0.6, width=max(1, len(data) // 100))
    ax4.axhline(y=0, color='black', linestyle='-', linewidth=0.8)
    ax4.set_xlabel('Generation', fontsize=12, fontweight='bold')
    ax4.set_ylabel('Cost Improvement', fontsize=12, fontweight='bold')
    ax4.set_title('Per-Generation Cost Improvement',
                  fontsize=13, fontweight='bold')
    ax4.grid(True, alpha=0.3, linestyle='--', axis='y')

    # 5. Statistics summary
    ax5 = fig.add_subplot(gs[2, 1])
    ax5.axis('off')

    stats_text = f"""
    EVOLUTION STATISTICS
    {'='*35}

    Total Generations:     {len(data):,}

    Initial Cost:          {initial_cost:.3f}
    Final Cost:            {final_cost:.3f}
    Total Improvement:     {initial_cost - final_cost:.3f}
    Improvement Rate:      {improvement:.2f}%

    Best Generation:       {data.loc[data['best_cost'].idxmin(), 'generation']:.0f}
    Worst Stagnation:      {data['solution_counter'].max():.0f} generations

    Avg Cost per Gen:      {data['best_cost'].mean():.3f}
    Cost Std Dev:          {data['best_cost'].std():.3f}
    """

    ax5.text(0.1, 0.95, stats_text, transform=ax5.transAxes,
             fontsize=11, verticalalignment='top', fontfamily='monospace',
             bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

    # Overall title
    fig.suptitle('CVRP Genetic Algorithm - Dynamic Evolution Visualization',
                 fontsize=18, fontweight='bold', y=0.98)

    # Save the figure
    plt.savefig(output_file, dpi=300, bbox_inches='tight', facecolor='white')
    print(f"✓ Evolution visualization saved to: {output_file}")

    # Show the plot
    plt.show()


def animate_evolution(csv_file='evolution_data.csv'):
    """
    Create an animated visualization that updates as the algorithm runs.
    """
    fig, ax = plt.subplots(figsize=(12, 6))

    def update(frame):
        ax.clear()
        if os.path.exists(csv_file):
            data = pd.read_csv(csv_file)
            ax.plot(data['generation'], data['best_cost'],
                   linewidth=2, color='#2E86AB')
            ax.set_xlabel('Generation', fontsize=12, fontweight='bold')
            ax.set_ylabel('Best Cost', fontsize=12, fontweight='bold')
            ax.set_title('Real-time Evolution Progress', fontsize=14, fontweight='bold')
            ax.grid(True, alpha=0.3)

    ani = FuncAnimation(fig, update, interval=1000, cache_frame_data=False)
    plt.show()


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == '--animate':
        print("Starting real-time animation...")
        animate_evolution()
    else:
        create_evolution_plots()
