#!/usr/bin/python3

import os.path
import networkx as nx
import powerlaw
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

from sys import argv
from networkx.drawing.nx_agraph import read_dot


if __name__ == '__main__':
    if len(argv) < 2 or not os.path.isdir(argv[1]):
        print('Error: first argument is no directory')
        exit()
    directory = argv[1]

    data_by_n = {}
    for filename in os.listdir(directory):
        if not filename[-4:] == '.dot':
            continue
        n = str(filename.split('_')[0])
        print('reading' + filename)
        g = nx.Graph(read_dot(os.path.join(directory, filename)))
        # gather data

        # numedges:
        # dat = len(g.edges())
        degrees = [len(nbrs) for n,nbrs in g.adj.items()]
        fit = powerlaw.Fit(degrees)
        dat = fit.alpha

        print('value is: ', dat)
        if not n in data_by_n:
            data_by_n[n] = []
        data_by_n[n] += [dat]

    list_of_n = sorted(data_by_n.keys())
    data = []
    for key in list_of_n:
        data += [data_by_n[key]]

    plt.boxplot(data, labels=list_of_n)
    plt.xlabel('n')
    plt.ylabel('powerlaw exponent')
    plt.savefig(os.path.join(directory, 'plot.pdf'))
    plt.clf()
