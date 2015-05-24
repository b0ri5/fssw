# Introduction #

This page is discussing parts of the implementation that require a little more attention.


## AddingGenerators ##

Schreier Trees are built through a sequence of `add_generator(g)` calls, by creating the path to the root from each element in its current orbit. Special attention is needed for orbits that are not yet known to contain the root, but show up in the algorithm early on. Thus all generators must be stored and (re-)processed later when a connection to the root is found:

  1. keep track of each generator added in a vector
  1. `build_tree` goes through each generator stored and performs a breadth-first search to find the orbit of the root
  1. `add_generator` will:
    * push back the generator to the vector
    * call `build_tree()`