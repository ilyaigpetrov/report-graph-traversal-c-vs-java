Requirements
============
To compile sources you need bash, gcc and some Unix utilites (dos2unix).
For Windows you may use Cygwin (better x86, not x64).
When installing it check packages `gcc-core` and `dos2unix`.


Inputs and Outputs
==================
Inputs for C must be placed into "./Code/C/Inputs" directory.
Inputs for Java must be placed into "./Code/Java/Inputs" directory.
Each input must be placed into subdirectory which name denotes number
of nodes in input graph. e.g., input files of 50-node graph must
be place into directory "./Code/C/Inputs/50" for C
or "./Code/Java/Inputs/50" for Java.
Java implementation has parser for target expressions.
C implementation doesn't have parser for target expressions.
Instead target expression must be put as C code into include
file "Expression.h" in input directory, e.g.,
into "./Code/C/Inputs/100/Expression.h" with contents like:
```
#define EXPRESSION CityIs("SouthAva") && pop == 20425427 && elevation <= 1074.0
```
You may use CityIs function to specify desired city name to be sought.


How to Compile and Run
======================
Compilation and execution may be done with bash scripts.
In Cygwin terminal:
```
cd /cygdrive/c/path_to_some_dir/Code
cd C
./compileAll.sh    # Compile sources.
# Now we need to convert line endings of all input files under ./Inputs directory to Unix style.
# This is needed because in sake of performance C implementation handles only Unix style line endings.
# Run the following script each time you add new input files in ./Inputs directory.
./prepareInputs.sh
./run.sh bfs 6     # To run BFS on ./Inputs/6 folder with 6 nodes of input.
./run.sh dfs 100 all # To run DFS on ./Inputs/100 folder with 100 nodes of input and make it not consider stop condition (traverses all nodes)
# Results are written to corresponding directories ./Inputs/6 or ./Inputs/100, e.g.
```
For Java the process is similar:
```
cd /cygdrive/c/path_to_some_dir/Code
cd Java
./compileAll.sh    # Compile sources.
./run.sh bfs 6     # To run BFS on ./Inputs/6 folder with 6 nodes of input.
./run.sh dfs 100 all # To run DFS on ./Inputs/100 folder with 100 nodes of input and make it not consider stop condition (traverses all nodes)
# Results are written to corresponding directories ./Inputs/6 or ./Inputs/100, e.g.
```

Charts
======
If you want to draw charts from outputs,
first generate some outputs, run `./run.sh` for
different combinations of (bfs|dfs) (6|100|1000|10000) and [all] flags.
Register on plot.ly,
install Python2 and plotly [ https://plot.ly/python/getting-started/ ].
Initialize plotly with credentials.
Then in Cygwin terminal:
```
cd /cygdrive/c/path_to_some_dir/Code
./AlgorithmPerformance.py # To draw algorithm performance charts.
./ReadPerformance.py # To draw read time performance charts.
```