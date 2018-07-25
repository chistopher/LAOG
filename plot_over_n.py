#!/usr/bin/python3

import os.path
import networkx as nx
import powerlaw
import matplotlib
matplotlib.use('Agg')
from matplotlib.ticker import MaxNLocator, FuncFormatter, EngFormatter
import matplotlib.pyplot as plt
import numpy as np

from sys import argv
from networkx.drawing.nx_agraph import read_dot
from statistics import mean, pstdev

# boxplot
def plot_stuff(all_data, key_to_plot):
    list_of_n = sorted(all_data.keys())
    data = [ all_data[n][key_to_plot] for n in list_of_n]

    plt.boxplot(data, labels=list_of_n)
    plt.xlabel('n')
    plt.ylabel(key_to_plot)
    plt.savefig(os.path.join(directory, 'plot_{}.pdf'.format(key_to_plot)))
    plt.clf()


# three lines (min max avg)
def plot_avg(all_data, key_to_plot, eng_fmt = False):
    list_of_n = sorted(all_data.keys())
    data_mean = [ mean(all_data[n][key_to_plot]) for n in list_of_n]
    data_min = [ min(all_data[n][key_to_plot]) for n in list_of_n]
    data_max = [ max(all_data[n][key_to_plot]) for n in list_of_n]
    data_dev = [ pstdev(all_data[n][key_to_plot]) for n in list_of_n]

    ax = plt.gca()
    ax2 = ax.twinx()

    ax.plot(list_of_n, data_mean, label="mean")
    ax.plot(list_of_n, data_min, label="min/max", color='#808080')
    ax.plot(list_of_n, data_max, color='#808080')
    ax2.plot(list_of_n, data_dev, label="stdev", color='#d62728')

    ax.fill_between(list_of_n, data_min, data_max, color='#539caf', alpha = 0.4)

    # x axis
    ax.set_xlabel('n')
    formatter = EngFormatter(sep="")
    ax.xaxis.set_major_formatter(formatter)
    # plt.xticks(np.arange(min(list_of_n), max(list_of_n)+1, 10000))

    # first y axis
    ax.set_ylabel(key_to_plot)
    if eng_fmt:
        ax.yaxis.set_major_formatter(formatter)
        ax.yaxis.set_major_locator(MaxNLocator(nbins='auto', integer=True))

    # second y axis
    ax1_range = max(data_max) - min(data_min)
    dev_middle = min(data_dev) + max(data_dev)/2
    ax2.set_ylim([dev_middle - ax1_range/2, dev_middle + ax1_range/2])
    no_negative = lambda x, pos=None: "" if x<0 else (formatter.format_eng(x) if eng_fmt else "%.2f" % x)
    ax2.yaxis.set_major_formatter(FuncFormatter(no_negative))

    ax.legend(loc=2)
    ax2.legend(loc=1)

    plt.savefig(os.path.join(directory, 'plot_{}.pdf'.format(key_to_plot)))
    plt.clf()


if __name__ == '__main__':
    if len(argv) < 2 or not os.path.isdir(argv[1]):
        print('Error: first argument is no directory')
        exit()
    directory = argv[1]

    data_by_n = {}
    for filename in os.listdir(directory):
        if not filename[-4:] == '.dot':
            continue
        n = int(filename.split('_')[0])

        # read file
        print('reading ', filename)
        totalname = os.path.join(directory, filename)
        with open(totalname) as file:
            metalines = [next(file) for x in range(13)]

        # gather data
        if not n in data_by_n:
            data_by_n[n] = {
                'rounds': [],
                'm': [],
                'maxDegree': [],
                'avgDegree': [],
                'clustering': [],
                'exponent': []
            }

        data_by_n[n]['rounds'].append(int(metalines[1].split()[-1]))
        data_by_n[n]['m'].append(int(metalines[5].split()[-1]))

        # continue
        # must read dot from here
        g = nx.Graph(read_dot(totalname))
        degrees = [len(nbrs) for n,nbrs in g.adj.items()]

        # maxDegree
        data_by_n[n]['maxDegree'].append(max(degrees))

        # avgDegree
        data_by_n[n]['avgDegree'].append(mean(degrees))

        # clustering
        data_by_n[n]['clustering'].append(nx.average_clustering(g))

        # pl exponent
        fit = powerlaw.Fit(degrees)
        data_by_n[n]['exponent'].append(fit.alpha)


    plot_avg(data_by_n, 'rounds', True)
    plot_avg(data_by_n, 'm', True)
    # exit()
    plot_avg(data_by_n, 'maxDegree', True)
    plot_avg(data_by_n, 'avgDegree', True)
    plot_avg(data_by_n, 'clustering')
    plot_avg(data_by_n, 'exponent')
