#!/usr/bin/python3

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.ticker import EngFormatter

xRange = [200, 400, 600, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000]
data = {
#2neigh
    "Naive (Alg 7.2 2neigh)": [402, 4159, 14178, 35161, 73263, 763223, 2614841],
    # 0: judge2: [402, 4159, 14178, 35161, 73263, 763223, 2614841 ...
    "Improved (Alg 7.3)": [17, 98, 243, 478, 822, 4809, 12349, 26779, 54287, 91981, 112163, 167289, 230196, 303233],
    # 1: judge2: [17, 98, 243, 478, 822, 4809, 12349, 26779, 54287, 91981, 112163, 167289, 230196, 303233]
    "Unrolled": [11, 63, 154, 305, 522, 3023, 7556, 15907, 32477, 54461, 65656, 97853, 136491, 178520],
    # 2: judge2: [11, 63, 154, 305, 522, 3023, 7556, 15907, 32477, 54461, 65656, 97853, 136491, 178520]
#dist
    "Naive (Alg 7.2 dist)": [394, 3910, 14847, 38900, 73367, 738372, 3165286],
    # 3: judge1: [394, 3910, 14847, 38900, 73367, 738372, 3165286 ...
    "only updated Nodes (Alg 7.4 no break)": [18, 95, 240, 470, 756, 3810, 10171, 20150, 33539, 50372, 76518, 110361, 144245, 180136],
    # 4: judge1: [18, 95, 240, 470, 756, 3810, 10171, 20150, 33539, 50372, 76518, 110361, 144245, 180136]
    "only updated Nodes (Alg 7.4)": [15, 80, 204, 401, 630, 3094, 8153, 16351, 27141, 40929, 60868, 85161, 112285, 140343],
    # 5: judge1: [15, 80, 204, 401, 630, 3094, 8153, 16351, 27141, 40929, 60868, 85161, 112285, 140343]
    "Purdom's Variation": [48, 245, 616, 1213, 1957, 10182, 26549, 51800, 85067, 126201, 190298, 272791, 356649, 443164],
    # 6: judge1: [48, 245, 616, 1213, 1957, 10182, 26549, 51800, 85067, 126201, 190298, 272791, 356649, 443164]
    "BFS in DAG": [29, 163, 420, 848, 1414, 8684, 25423, 53034, 100077, 171172, 283498, 432476, 616548, 820315],
    # 7: judge1: [29, 163, 420, 848, 1414, 8684, 25423, 53034, 100077, 171172, 283498, 432476, 616548, 820315]
    "BFS in DAG static Memory": [18, 113, 312, 632, 1046, 7225, 21949, 46129, 89449, 158425, 264576, 402786, 577796, 774977],
    # 8: judge1: [18, 113, 312, 632, 1046, 7225, 21949, 46129, 89449, 158425, 264576, 402786, 577796, 774977]
}

def plot(keyName, ax, high):
    yValues = data[keyName]
    # ax.plot(xRange[:len(yValues)], [y/1000 for y in yValues], label=keyName, marker='.')
    if high:
        yValues = yValues[4:]
        xValues = xRange[4:4+len(yValues)]
    else:
        yValues = yValues[:5]
        xValues = xRange[:5]
    ax.plot(xValues, [y/1000 for y in yValues], label=keyName, marker='.')


def plot_all(keys, suffix):
    for high in [False, True]:
        ax = plt.gca()
        ax.set_xlabel('n')
        ax.set_ylabel('time in seconds')
        if high:
            ax.xaxis.set_major_formatter(EngFormatter(sep=""))
            ax.set_yscale('log')
        for key in keys:
            plot(key, ax, high)
        ax.legend()
        # ax.legend(loc=2)
        plt.savefig("benchmark-{}.pdf".format(suffix+str(int(high))))
        plt.clf()


if __name__ == '__main__':

    neighs = list(data.keys())[:3]
    dists = list(data.keys())[3:]

    plot_all(neighs, "neigh")
    plot_all(dists, "dist")
