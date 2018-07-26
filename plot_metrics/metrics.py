
import json
import os.path
import powerlaw
from statistics import mean
from graph_tool import load_graph
from graph_tool.clustering import local_clustering, global_clustering
from graph_tool.topology import pseudo_diameter


def metrics(file, use_cache=True):
    # use cache or recompute
    cache = os.path.splitext(file)[0] + ".json"
    if use_cache and os.path.isfile(cache):
        print('using cached metrics for', os.path.basename(file))
        with open(cache, "r") as fp:
            return json.load(fp)
    print('computing metrics for', os.path.basename(file))

    # read file
    g = load_graph(file)
    degrees = list(g.degree_property_map("out"))
    with open(file) as f:
        metalines = [next(f) for x in range(13)]

    # gather data
    metrics = {}
    metrics['file'] = os.path.basename(file)
    metrics['edges'] = int(metalines[5].split()[-1])
    metrics['rounds'] = int(metalines[1].split()[-1])
    metrics['max_degree'] = max(degrees)
    metrics['avg_degree'] = mean(degrees)
    metrics['min_degree'] = min(degrees)
    metrics['local_clustering'] = mean(local_clustering(g).get_array())
    metrics['global_clustering'] = global_clustering(g)[0]
    metrics['pseudo_diameter'] = int(pseudo_diameter(g)[0])
    fit = powerlaw.Fit(degrees, discrete=True, verbose=False)
    metrics['exponent'] = fit.alpha
    metrics['KS'] = fit.power_law.KS()
    metrics['x_min'] = fit.xmin

    with open(cache, "w") as fp:
        json.dump(metrics, fp)

    return metrics
