#!/usr/bin/env python3
"""
Visualization script for CVRP solution routes.
Creates beautiful 2D plots showing vehicle routes with different colors.
"""

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.collections import LineCollection
import numpy as np
import sys
import os
import re


def read_vrp_file(vrp_file):
    """
    Read VRP problem file and extract node coordinates.

    Args:
        vrp_file: Path to the .vrp file

    Returns:
        dict: Node coordinates {node_id: (x, y)}
        dict: Problem info (dimension, capacity)
    """
    coords = {}
    info = {}

    with open(vrp_file, 'r') as f:
        lines = f.readlines()

    # Parse dimension and capacity
    for line in lines[:10]:
        if 'DIMENSION' in line:
            info['dimension'] = int(line.split(':')[1].strip())
        elif 'CAPACITY' in line:
            info['capacity'] = int(line.split(':')[1].strip())

    # Parse coordinates
    coord_section = False
    demand_section = False

    for line in lines:
        line = line.strip()

        if 'NODE_COORD_SECTION' in line:
            coord_section = True
            continue
        elif 'DEMAND_SECTION' in line:
            coord_section = False
            demand_section = True
            continue
        elif not line or line.startswith('EOF'):
            break

        if coord_section:
            parts = line.split()
            if len(parts) == 3:
                node_id = int(parts[0])
                x, y = float(parts[1]), float(parts[2])
                coords[node_id] = (x, y)

    return coords, info


def read_solution_file(solution_file):
    """
    Read solution file and extract routes.

    Args:
        solution_file: Path to the solution file (e.g., best-solution.txt)

    Returns:
        list: List of routes, where each route is a list of node IDs
        float: Total cost
    """
    routes = []
    cost = 0.0

    with open(solution_file, 'r') as f:
        lines = f.readlines()

    for line in lines:
        line = line.strip()

        # Extract cost
        if line.startswith('cost'):
            cost = float(line.split()[1])

        # Extract routes (format: 1->node1->node2->...->1)
        elif '->' in line:
            nodes = [int(n) for n in line.split('->')]
            # Remove duplicate depot (1) at start and end for internal representation
            route = nodes[1:-1] if len(nodes) > 2 else []
            if route:  # Only add non-empty routes
                routes.append(route)

    return routes, cost


def visualize_routes(vrp_file='fruitybun250.vrp', solution_file='best-solution.txt',
                     output_file='routes_visualization.png'):
    """
    Create beautiful visualization of CVRP routes.

    Args:
        vrp_file: Path to the VRP problem file
        solution_file: Path to the solution file
        output_file: Path to save the output image
    """

    # Check if files exist
    if not os.path.exists(vrp_file):
        print(f"Error: {vrp_file} not found!")
        return
    if not os.path.exists(solution_file):
        print(f"Error: {solution_file} not found!")
        return

    # Read data
    coords, info = read_vrp_file(vrp_file)
    routes, cost = read_solution_file(solution_file)

    # Create figure
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 10))

    # Color palette for routes
    colors = plt.cm.tab20(np.linspace(0, 1, len(routes)))
    if len(routes) > 20:
        colors = plt.cm.gist_rainbow(np.linspace(0, 1, len(routes)))

    depot_coord = coords[1]

    # ========== Left plot: All routes with depot ==========
    ax1.set_title(f'CVRP Solution: {len(routes)} Vehicle Routes\nTotal Cost: {cost:.2f}',
                  fontsize=16, fontweight='bold', pad=20)

    # Plot all customer nodes
    customer_coords = [coords[i] for i in coords if i != 1]
    if customer_coords:
        cx, cy = zip(*customer_coords)
        ax1.scatter(cx, cy, c='lightblue', s=80, alpha=0.6,
                   edgecolors='darkblue', linewidths=1.5, label='Customers', zorder=2)

    # Plot depot
    ax1.scatter(depot_coord[0], depot_coord[1], c='red', s=500,
               marker='*', edgecolors='darkred', linewidths=2,
               label='Depot', zorder=5)

    # Plot each route
    for idx, route in enumerate(routes):
        # Create route coordinates (depot -> customers -> depot)
        route_coords = [depot_coord]
        for node in route:
            if node in coords:
                route_coords.append(coords[node])
        route_coords.append(depot_coord)

        # Draw route path
        if len(route_coords) > 1:
            rx, ry = zip(*route_coords)
            ax1.plot(rx, ry, color=colors[idx], linewidth=2.5,
                    alpha=0.7, zorder=1)

            # Add arrows to show direction
            for i in range(len(rx) - 1):
                dx = rx[i + 1] - rx[i]
                dy = ry[i + 1] - ry[i]
                ax1.arrow(rx[i], ry[i], dx * 0.7, dy * 0.7,
                         head_width=3, head_length=2,
                         fc=colors[idx], ec=colors[idx],
                         alpha=0.5, zorder=1, length_includes_head=True)

    ax1.set_xlabel('X Coordinate', fontsize=12, fontweight='bold')
    ax1.set_ylabel('Y Coordinate', fontsize=12, fontweight='bold')
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.legend(fontsize=11, loc='upper right')
    ax1.set_aspect('equal', adjustable='box')

    # ========== Right plot: Individual route statistics ==========
    ax2.set_title('Route Statistics & Distribution',
                  fontsize=16, fontweight='bold', pad=20)

    # Calculate route lengths and customer counts
    route_lengths = []
    route_customers = []

    for route in routes:
        # Calculate route length
        length = 0
        prev_node = 1  # Start from depot
        for node in route:
            if node in coords and prev_node in coords:
                x1, y1 = coords[prev_node]
                x2, y2 = coords[node]
                length += np.sqrt((x2 - x1)**2 + (y2 - y1)**2)
            prev_node = node
        # Return to depot
        if prev_node in coords:
            x1, y1 = coords[prev_node]
            x2, y2 = depot_coord
            length += np.sqrt((x2 - x1)**2 + (y2 - y1)**2)

        route_lengths.append(length)
        route_customers.append(len(route))

    # Create bar charts
    route_ids = list(range(1, len(routes) + 1))

    # Subplot 1: Route lengths
    ax2_sub1 = plt.subplot(2, 2, 2)
    bars1 = ax2_sub1.bar(route_ids, route_lengths, color=colors, alpha=0.7, edgecolor='black')
    ax2_sub1.set_xlabel('Route ID', fontsize=11, fontweight='bold')
    ax2_sub1.set_ylabel('Route Length', fontsize=11, fontweight='bold')
    ax2_sub1.set_title('Route Lengths', fontsize=12, fontweight='bold')
    ax2_sub1.grid(True, alpha=0.3, axis='y')
    ax2_sub1.axhline(y=np.mean(route_lengths), color='red', linestyle='--',
                     linewidth=2, label=f'Avg: {np.mean(route_lengths):.1f}')
    ax2_sub1.legend(fontsize=9)

    # Subplot 2: Customers per route
    ax2_sub2 = plt.subplot(2, 2, 4)
    bars2 = ax2_sub2.bar(route_ids, route_customers, color=colors, alpha=0.7, edgecolor='black')
    ax2_sub2.set_xlabel('Route ID', fontsize=11, fontweight='bold')
    ax2_sub2.set_ylabel('Number of Customers', fontsize=11, fontweight='bold')
    ax2_sub2.set_title('Customers per Route', fontsize=12, fontweight='bold')
    ax2_sub2.grid(True, alpha=0.3, axis='y')
    ax2_sub2.axhline(y=np.mean(route_customers), color='red', linestyle='--',
                     linewidth=2, label=f'Avg: {np.mean(route_customers):.1f}')
    ax2_sub2.legend(fontsize=9)

    # Remove the main right plot axes
    ax2.axis('off')

    # Add statistics text box
    stats_text = f"""
    SOLUTION STATISTICS
    {'='*30}

    Total Routes:          {len(routes)}
    Total Customers:       {sum(route_customers)}
    Total Cost:            {cost:.2f}

    Avg Route Length:      {np.mean(route_lengths):.2f}
    Min Route Length:      {np.min(route_lengths):.2f}
    Max Route Length:      {np.max(route_lengths):.2f}

    Avg Customers/Route:   {np.mean(route_customers):.1f}
    Min Customers/Route:   {np.min(route_customers)}
    Max Customers/Route:   {np.max(route_customers)}

    Vehicle Capacity:      {info.get('capacity', 'N/A')}
    Problem Dimension:     {info.get('dimension', 'N/A')}
    """

    fig.text(0.68, 0.35, stats_text, fontsize=11, fontfamily='monospace',
             verticalalignment='top',
             bbox=dict(boxstyle='round', facecolor='lightgreen', alpha=0.3))

    # Overall title
    fig.suptitle('CVRP Solution Visualization - Vehicle Routing Result',
                 fontsize=18, fontweight='bold', y=0.98)

    # Save and show
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig(output_file, dpi=300, bbox_inches='tight', facecolor='white')
    print(f"✓ Route visualization saved to: {output_file}")

    plt.show()


if __name__ == '__main__':
    vrp_file = 'fruitybun250.vrp'
    solution_file = 'best-solution.txt'

    if len(sys.argv) > 1:
        vrp_file = sys.argv[1]
    if len(sys.argv) > 2:
        solution_file = sys.argv[2]

    visualize_routes(vrp_file, solution_file)
