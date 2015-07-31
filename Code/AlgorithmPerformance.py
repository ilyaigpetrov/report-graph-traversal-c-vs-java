#!/usr/bin/env python
import os
import json
implementations = ["C", "Java"]
algorithms = ["DFS", "BFS"]

measurements = {}
for impl in implementations:
    measurements[impl] = {}
    for algo in algorithms:
        measurements[impl][algo] = {}
        inputsDir = os.path.join(impl, "Inputs")
        for nodesCount in os.listdir(inputsDir):
            profileFile = os.path.join(inputsDir, nodesCount, "Profile_of_{0}_from_Source_to_Target_N_{1}.txt".format(algo, nodesCount))
            if not os.path.exists(profileFile):
                print("No profile for {0}/{1}".format(nodesCount, algo))
            else:
                text = open(profileFile, "r").read()
                data = json.loads(text)
                measurements[impl][algo][int(nodesCount)] = data
#
# impl:
#   algo:
#     nodesCount:
#

from collections import *
result = {}
for impl in implementations:
    values = OrderedDict()
    for algo in algorithms:
        for nodesCount, profile in sorted(measurements[impl][algo].iteritems()):
            values[algo.lower()+str(nodesCount)] = profile["ALGO:"+algo] or 1
    result[impl] = OrderedDict([(k, values[k]) for k in sorted(values, key = lambda v: int(v[3:]))])
        
# result:
#   Java:
#     6DFS: ms
#     6BFS: ms
#     100DFS: ms

import plotly.plotly as py
from plotly.graph_objs import *

traces = []
for impl in implementations:  
    trace = Bar(
        x = result[impl].keys(),
        y = result[impl].values(),
        name = impl
    )
    traces.append(trace)
    
data = Data(traces)
layout = Layout(
    title='Algorithms: C vs Java',
    barmode='group',
    yaxis=YAxis(
        title='Milliseconds'
    )
)
fig = Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='Algorithms-Java-C')
