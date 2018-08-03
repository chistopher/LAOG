#!/usr/bin/python3

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.ticker import EngFormatter

xRange = [200, 400, 600, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000]
data = {
#2neigh
    "Naive (Alg 7.2 2neigh)": [112, 1118],
    "Improved (Alg 7.3)": [10, 64, 155, 311, 550],
#dist
    "Naive (Alg 7.2 dist)": [253, 2447, 9725],
    "only updated Nodes (Alg 7.4 no break)": [20, 109, 275, 546, 930],
    "only updated Nodes (Alg 7.4)": [16, 87, 216, 424, 696],
    "Purdom's Variation": [57, 324, 747, 1536, 2525],
    "BFS in DAG": [34, 177, 428, 831, 1406],
    "BFS in DAG static Memory": [19, 115, 288, 575, 975],
}


def plot(keyName, ax):
    yValues = data[keyName]
    ax.plot(xRange[:len(yValues)], [y/1000 for y in yValues], label=keyName)


if __name__ == '__main__':
    ax = plt.gca()
    # axes
    ax.xaxis.set_major_formatter(EngFormatter(sep=""))
    ax.set_xlabel('n')
    # ax.set_yscale('log')
    ax.set_ylabel('time in seconds')

    #plot("Naive (Alg 7.2 2neigh)", ax)
    #plot("Improved (Alg 7.3)", ax)
    plot("Naive (Alg 7.2 dist)", ax)
    plot("only updated Nodes (Alg 7.4 no break)", ax)
    plot("only updated Nodes (Alg 7.4)", ax)
    plot("Purdom's Variation", ax)
    plot("BFS in DAG", ax)
    plot("BFS in DAG static Memory", ax)

    ax.legend()
    # ax.legend(loc=2)

    plt.savefig("benchmark.pdf")
    plt.clf()
