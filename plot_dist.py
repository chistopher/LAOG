#!/usr/bin/python3

import sys
import os.path
import numpy as np
import networkx as nx
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

from sys import argv
from networkx.drawing.nx_agraph import read_dot


if __name__ == '__main__':
    if len(argv) < 2 or not os.path.isfile(argv[1]):
        print('Error: first argument is no file')
        exit()
    g = nx.Graph(read_dot(argv[1]))
    degrees = [len(nbrs) for n,nbrs in g.adj.items()]
    dist = np.bincount(degrees)
    cdf = np.cumsum(dist)/np.sum(dist)
    ccdf = 1-cdf
    plt.plot(range(len(ccdf)),ccdf,'k.')
    plt.xscale('log')
    plt.yscale('log')
    plt.ylabel('CCDF')
    plt.xlabel('Degree')
    plt.savefig(argv[1][:-4] + '_ccdf.pdf')
    plt.clf()
