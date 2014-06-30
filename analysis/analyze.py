#!/usr/bin/env python3

import os
import sys
import re
import matplotlib.pyplot as plt
import numpy as np

try:
    import seaborn as sns
except ImportError:
    pass

def analyze(data, accepted, meta, drop=0, skip=100):
    data = np.array(data)[drop:]
    data_count = len(data)
    # shift data
    #data -= np.mean, (data)

    text = []
    for key, value in meta.items():
        text.append("%s: %s" % (key, value))

    plt.subplot(221)
    plt.title("Histogram of encountered energy values")
    n, bins, patches = plt.hist(data, 50, facecolor='red')
    plt.plot(bins)
    plt.xlabel('Energy [a.u.]')
    plt.ylabel('Occurences')
    plt.xlim(bins.min(), bins.max())

    plt.subplot(222)
    plt.title("Encountered energy values over time")
    plt.plot(np.arange(0, data_count, skip), data[::skip], '.')
    plt.xlim(0, data_count)
    locs, labels = plt.xticks()
    #plt.xticks(locs, ["%.2E" % (x*steps) for x in locs])
    plt.xlabel("Iterations")
    plt.ylabel("Energy [a.u.]")

    # filter out rejected outfnalues
    accepted = np.array(accepted, dtype=bool)[drop:]
    indices = accepted.nonzero()[0]
    data = data[accepted]
    accepted_count = len(data)

    plt.subplot(223)
    plt.title("Histogram of accepted energy values")
    n, bins, patches = plt.hist(data, 50, facecolor='red')
    plt.plot(bins)
    plt.xlabel('Energy [a.u.]')
    plt.ylabel('Occurences')
    plt.xlim(bins.min(), bins.max())

    plt.subplot(224)
    plt.title("Accepted energy values over time")
    plt.plot(indices, data, '.')
    plt.xlim(0, data_count)
    plt.xlabel("Iterations")
    plt.ylabel("Energy [a.u.]")

    text.append("percent accepted: %.4f%%" %
                ((accepted_count / data_count) * 100))
    text.append("min = %f a.u." % np.amin(data))
    text.append("µ = %f a.u." % np.mean(data))
    plt.suptitle(" :: ".join(text))

    plt.show()


def analyze_errfile(err_fn):
    data = []
    meta = {}
    with open(err_fn) as fin:
        n = 0
        for line in fin:
            m = re.match(
                "\[(\d+)\]\s+([AR])\s+([\d\.\-]+)\s+\[ΔE=([\d\.\-]+)\]\s*", line)
            if m:
                n, ar, E, dE = m.groups()
                data.append((int(n), ar, float(E), float(dE)))
            m = re.match(r"^DEBUG:\sStarting simulation at ([\d\-]+ [\d:]+) on (\w+):$",
                         line)
            if m:
                date, host = m.groups()
                meta['date'] = date
                meta['host'] = host
            m = re.match(r"^\s+max_dist\s+=\s+([\d\.]+)", line)
            if m:
                meta['max_dist'] = m.groups(1)[0]
            m = re.match(r"^\s+max_angle\s+=\s+([\d\.]+)", line)
            if m:
                meta['max_angle'] = m.groups(1)[0]
            m = re.match(r"^\s+RT ratio\s+=\s+([\d\.]+)", line)
            if m:
                meta['RT ratio'] = m.groups(1)[0]
    energy = []
    status = []
    for n, s, E, dE in data:
        energy.append(E)
        status.append(True) if s == "A" else status.append(False)
    analyze(energy, status, meta)

if __name__ == "__main__":
    analyze_errfile(sys.argv[1])

