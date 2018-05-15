#!/usr/bin/python3

import os.path

import numpy
import powerlaw
import networkx as nx
from networkx.drawing.nx_agraph import read_dot

from sys import argv

if __name__ == '__main__':
    if len(argv) < 2 or not os.path.isfile(argv[1]):
        print('Error: first argument is no file')
        exit()
    g = nx.Graph(read_dot(argv[1]))
    degrees = [len(nbrs) for n,nbrs in g.adj.items()]
    print('nodes                ', len(g))
    print('edges                ', len(g.edges()))
    print('max degree           ', max(degrees))
    # print('diameter             ', nx.diameter(g))
    print('average clustering   ', nx.average_clustering(g))
    numpy.seterr(divide='ignore', invalid='ignore')
    fit = powerlaw.Fit(degrees)
    print('power law exponent   ', fit.alpha)
    print('xmin                 ', fit.xmin)
    print('%nodes under xmin    ', 100 * sum(i < fit.xmin for i in degrees) / float(len(g)))
    print('KS distance          ', fit.power_law.KS())
    print('power law likelihood ', fit.distribution_compare('power_law', 'exponential'))
