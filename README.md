# Minimum Weight Vertex Cover

The MWVC is an optimization problem of given a graph G=(V,E) and a weight function w:V->R, find a vertex cover C of G that minimizes the sum of weights of the vertex in C.

This repository has a set of strategies to solve this problem including local searches, simulated annealing, genetic algorithm and a variable neighborhood search. 

The most elaborated strategy is the VNS.

## Organization

The root directory has two folders. The "instances" folder contains the instances(folder "insts") used for tests and the code used to generate the instances. The "solver" folder contains the code distributed in cpp and hpp files and an "include" folder, and executable "MAE-HTS.exe" wich is the state-of-art for the problem(obtained from https://github.com/HustWangYang/MAE-HTS) and a folder "results" wich contains the results organized by approach type and a pdf file wich contains the formatted results.

## Compile

Inside solver folder.

### Debug build

```bashsession
user~$: mkdir Debug && cd Debug
user~$: cmake DCMAKE_BUILD=Debug ..
user~$: make
```

### Release build

```bashsession
user~$: mkdir Release && cd Release
user~$: cmake DCMAKE_BUILD=Release ..
user~$: make
```
## Run

```bashsession
./main [INSTANCE_PATH] [Program Options]
```

### Program Options

```bashsession
./main --help
```
