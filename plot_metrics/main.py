#!/usr/bin/python3

import json
import os
from sys import argv

from analyze import analyze
from plot import plot


if __name__ == '__main__':
    if len(argv) < 2 or not os.path.isdir(argv[1]):
        print('Error: first argument is no directory')
        exit()
    directory = argv[1]


    all_data = analyze(directory, use_cache=True)

    plot(all_data, "edges", directory, True)
    plot(all_data, "rounds", directory)
    plot(all_data, "max_degree", directory, True)
    plot(all_data, "avg_degree", directory)
    plot(all_data, "min_degree", directory)
    plot(all_data, "local_clustering", directory)
    plot(all_data, "global_clustering", directory)
    plot(all_data, "pseudo_diameter", directory)
    plot(all_data, "exponent", directory)
    plot(all_data, "KS", directory)
    plot(all_data, "x_min", directory)
