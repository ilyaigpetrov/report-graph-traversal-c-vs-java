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
            profileFile = os.path.join(inputsDir, nodesCount, "Profile_of_{0}_from_Source_to_Target_N_1.txt".format(algo))
            if not os.path.exists(profileFile):
                print("No profile for {}/{}/{}".format(impl, nodesCount, algo))
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
            values[nodesCount] = values.get(nodesCount) or 0
            values[nodesCount] += profile["INPUT"]
    for nodesCount in values.keys():
        values[nodesCount] /= float(len(algorithms))
        values[nodesCount] = values[nodesCount] or 1
    # values:
    #   nodesCount: DFS + BFS / 2
    result[impl] = values
        

# result:
#   Java:
#     6: DFS + BFS / 2 ms
#     100: DFS + BFS / 2 ms
#   C:
#     6: DFS + BFS / 2 ms
#     100: DFS + BFS / 2 ms

import plotly.plotly as py
from plotly.graph_objs import *

traces = []
for impl in implementations:  
    trace = Bar(
        x = [str(a)+" nodes" for a in result[impl].keys()],
        y = result[impl].values(),
        name = impl
    )
    traces.append(trace)
    
data = Data(traces)
layout = Layout(
    title='Input Reading: C vs Java',
    barmode='group',
    yaxis=YAxis(
        title='Milliseconds'
    )
)
fig = Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='Input-Reading-Java-C')
