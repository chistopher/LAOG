
import json
import os

def analyze(directory, use_cache=True):
    # use cache or recompute
    cache = os.path.join(directory, "metrics.json")
    if use_cache and os.path.isfile(cache):
        print('using cached metrics for', directory)
        with open(cache, "r") as fp:
            # load json and convert keys back to int 
            return {int(k): v for k,v in json.load(fp).items()}
    print('computing metrics for', directory)

    # import here coz it takes consierable amount of time (>1sec)
    from metrics import metrics

    # search dot files
    dots_by_n = {}
    for file in os.listdir(directory):
        if not file[-4:] == '.dot':
            continue
        n = int(file.split('_')[0])
        if n not in dots_by_n:
            dots_by_n[n] = []
        dots_by_n[n].append(os.path.join(directory, file))

    # validate data
    samples_per_n = len(next(iter(dots_by_n.values())))
    if False in (len(files) == samples_per_n for files in dots_by_n.values()):
        print("not all n have same number of samples!")
        exit()

    #calculate metrics
    all_data = {n: [metrics(file, use_cache) for file in files] for (n, files) in dots_by_n.items()}

    # update cache
    with open(cache, "w") as fp:
        json.dump(all_data, fp)

    return all_data
