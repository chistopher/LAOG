
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator, FuncFormatter, EngFormatter
from statistics import mean, pstdev
import os.path


def plot(all_data, key_to_plot, directory=".", eng_fmt = False):
    list_of_n = sorted(all_data.keys())
    samples =  len(next(iter(all_data.values())))
    data = [ [all_data[n][i][key_to_plot] for i in range(samples)] for n in list_of_n]
    data_mean = list(map(mean, data))
    data_min = list(map(min, data))
    data_max = list(map(max, data))
    data_dev = list(map(pstdev, data))

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
