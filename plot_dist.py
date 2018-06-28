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


def plot(data, ylabel, name):
    plt.plot(range(len(data)),data,'k.')
    plt.xscale('log')
    plt.yscale('log')
    plt.ylabel(ylabel)
    plt.xlabel('Degree')
    plt.savefig(name)
    plt.clf()


if __name__ == '__main__':
    if len(argv) < 2 or not os.path.isfile(argv[1]):
        print('Error: first argument is no file')
        exit()
    g = nx.Graph(read_dot(argv[1]))
    degrees = [len(nbrs) for n,nbrs in g.adj.items()]

    dist = np.bincount(degrees)
    cdf = np.cumsum(dist)/np.sum(dist)
    ccdf = 1-cdf

    #plot(dist, 'frequency', argv[1][:-4] + '_dist.pdf')
    #plot(cdf, 'CDF', argv[1][:-4] + '_cdf.pdf')
    plot(ccdf, 'CCDF', argv[1][:-4] + '_ccdf.png')
