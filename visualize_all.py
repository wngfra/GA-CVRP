#!/usr/bin/env python3
"""
Main visualization script for GA-CVRP.
Runs both evolution and route visualizations.
"""

import sys
import os
import subprocess
import argparse


def check_dependencies():
    """Check if required Python packages are installed."""
    try:
        import matplotlib
        import numpy
        import pandas
        print("✓ All required packages are installed")
        return True
    except ImportError as e:
        print(f"✗ Missing required package: {e}")
        print("\nPlease install required packages:")
        print("  pip install -r requirements.txt")
        return False


def run_visualizations(evolution=True, routes=True, vrp_file='fruitybun250.vrp',
                      solution_file='best-solution.txt'):
    """
    Run visualization scripts.

    Args:
        evolution: Generate evolution visualization
        routes: Generate route visualization
        vrp_file: Path to VRP problem file
        solution_file: Path to solution file
    """

    print("\n" + "="*60)
    print("  GA-CVRP Visualization Suite")
    print("="*60 + "\n")

    success_count = 0
    total_count = 0

    if evolution:
        total_count += 1
        print("📊 Generating evolution visualization...")
        try:
            from visualize_evolution import create_evolution_plots
            create_evolution_plots()
            success_count += 1
        except Exception as e:
            print(f"✗ Error generating evolution visualization: {e}")

    if routes:
        total_count += 1
        print("\n🗺️  Generating route visualization...")
        try:
            from visualize_routes import visualize_routes
            visualize_routes(vrp_file, solution_file)
            success_count += 1
        except Exception as e:
            print(f"✗ Error generating route visualization: {e}")

    print("\n" + "="*60)
    print(f"✓ Completed {success_count}/{total_count} visualizations successfully")
    print("="*60 + "\n")


def main():
    parser = argparse.ArgumentParser(
        description='GA-CVRP Visualization Suite',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python visualize_all.py                    # Generate all visualizations
  python visualize_all.py --evolution-only   # Only evolution visualization
  python visualize_all.py --routes-only      # Only route visualization
  python visualize_all.py --vrp myfile.vrp   # Use custom VRP file
        """)

    parser.add_argument('--evolution-only', action='store_true',
                       help='Generate only evolution visualization')
    parser.add_argument('--routes-only', action='store_true',
                       help='Generate only route visualization')
    parser.add_argument('--vrp', default='fruitybun250.vrp',
                       help='VRP problem file (default: fruitybun250.vrp)')
    parser.add_argument('--solution', default='best-solution.txt',
                       help='Solution file (default: best-solution.txt)')
    parser.add_argument('--check-deps', action='store_true',
                       help='Check if dependencies are installed')

    args = parser.parse_args()

    if args.check_deps:
        check_dependencies()
        return

    if not check_dependencies():
        return

    # Determine which visualizations to run
    evolution = not args.routes_only
    routes = not args.evolution_only

    run_visualizations(evolution, routes, args.vrp, args.solution)


if __name__ == '__main__':
    main()
