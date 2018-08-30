#!/usr/bin/python3

# prints metrics of a .dot file
# plots pdf cdf and ccdf if additional parameter is supplied
# uncomment diam and clustering if necessary;
# for large files use the analyze function in plot_metrics/analyze (uses graph-tools c++ binding instead of networkx)


import os.path

import numpy
import powerlaw
import networkx as nx
from networkx.drawing.nx_agraph import read_dot

from sys import argv

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

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
    # print('average clustering   ', nx.average_clustering(g))
    numpy.seterr(divide='ignore', invalid='ignore')
    fit = powerlaw.Fit(degrees, discrete=True, verbose=False)
    print('power law exponent   ', fit.alpha)
    print('xmin                 ', fit.xmin)
    print('%nodes under xmin    ', 100 * sum(i < fit.xmin for i in degrees) / float(len(g)))
    print('KS distance          ', fit.power_law.KS())
    print('compare with exponential distribution ... ')
    comp = fit.distribution_compare('power_law', 'exponential')
    print('loglikelihood ratio  ', comp[0])
    print('p-value              ', comp[1])

    if len(argv) > 2:
        fig = fit.plot_pdf(original_data=True, color='b', linewidth=2, label='Empirical Data')
        fig.axvline(x=fit.xmin, color='k', linestyle='--', linewidth=1, label='xmin')
        fit.power_law.plot_pdf(color='b', ax=fig, linestyle='--', linewidth=1, label='Power Law Fit')
        bottom, top = fig.get_ylim()
        #fit.lognormal.plot_pdf(color='r', ax=fig, linestyle='--', linewidth=1, label='Log-normal Fit')
        fit.exponential.plot_pdf(color='g', ax=fig, linestyle='--', linewidth=1, label='Exponential Fit')
        fig.set_ylim(bottom=bottom)
        fig.legend()
        plt.savefig(argv[1][:-4] + '_pdf.pdf')
        plt.clf()

        fig = fit.plot_cdf(original_data=True, color='b', linewidth=2, label='Empirical Data')
        fig.axvline(x=fit.xmin, color='k', linestyle='--', linewidth=1, label='xmin')
        fit.power_law.plot_cdf(color='b', ax=fig, linestyle='--', linewidth=1, label='Power Law Fit')
        bottom, top = fig.get_ylim()
        #fit.lognormal.plot_cdf(color='r', ax=fig, linestyle='--', linewidth=1, label='Log-normal Fit')
        fit.exponential.plot_cdf(color='g', ax=fig, linestyle='--', linewidth=1, label='Exponential Fit')
        fig.set_ylim(bottom=bottom)
        fig.legend()
        plt.savefig(argv[1][:-4] + '_cdf.pdf')
        plt.clf()

        fig = fit.plot_ccdf(original_data=True, color='b', linewidth=2, label='Empirical Data')
        fig.axvline(x=fit.xmin, color='k', linestyle='--', linewidth=1, label='xmin')
        fit.power_law.plot_ccdf(color='b', ax=fig, linestyle='--', linewidth=1, label='Power Law Fit')
        bottom, top = fig.get_ylim()
        #fit.lognormal.plot_ccdf(color='r', ax=fig, linestyle='--', linewidth=1, label='Log-normal Fit')
        fit.exponential.plot_ccdf(color='g', ax=fig, linestyle='--', linewidth=1, label='Exponential Fit')
        fig.set_ylim(bottom=bottom)
        fig.legend()
        plt.savefig(argv[1][:-4] + '_ccdf.pdf')
        plt.clf()
