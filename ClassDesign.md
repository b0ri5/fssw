# Introduction #

This page is for sketching the classes that will be needed and what methods they should have.

# Classes #

A permutation only needs to know what maps what to what, and how to compose itself with
another
```
class Permutation
  virtual int get_image(int a) const = 0;
  virtual int get_inverse_image(int a) const = 0;
```

A map permutation is the basic implementation of Permutation. It only stores the images of the moved elements. Composition changes the mapping.
```
class MapPermutation : Permutation
  bool from_string(string s);
  map<int, int> images_;
  map<int, int> inverse_images_;
```

A permutation word is a sequence _g<sub>1</sub>_ `*` _g<sub>2</sub>_ `*` ... `*` _g<sub>m</sub>_ where each _g<sub>i</sub>_ is either a permutation object or its inverse. Composition just appends a new element.
```
class PermutationWord : Permutation
  bool from_string(string s);
  void compose(const PermutationWord/MapPermutation &g);
  void evaluate(MapPermutation *g_ptr);
  vector<PermutationPart> permutations_;
```
where a `PermutationPart` is a utility struct grouping a permutation word and its direction (whether it is inverse):
```
struct PermutationPart
  const MapPermutation *g_ptr;
  bool is_inverse;
```
Permutation words can be composed with both map permutations and other permutation words.

A schreier tree just stores the mapping from an element and the permutation that will eventually take it to the root. This stores inverse transversal permutations for each element in the orbit of the root. It is built from the root through add\_generator(g) as described in [ImplementationDetails#AddingGenerators](ImplementationDetails#AddingGenerators.md).
```
class SchreierTree
  void set_root(int root);
  bool is_minimal(int a);
  bool is_in_orbit(int a);
  bool add_generator(const PermutationWord &g);
  bool path_to_root(int a, PermutationWord *path_ptr);
  map<int, const PermutationWord *> tree_;
  int root_;
```

The fundamental Schreier trees for a given base. The word "fundamental" comes from the fundamental orbits, which are the orbits of each base element after fixing all previous base elements.

```
class FundamentalSchreierTrees
  int strip(const Permutation &g, PermutationWord *h_ptr)

  void append_to_base(int b);

  // adds "w" to each schreier tree up until "w" moves a base element
  void add_generator(const PermutationWord *w_ptr);

  // wraps MapPermutation in a PermutationWord of length 1
  void add_generator(const MapPermutation *g_ptr);

  vector<int> base_;
  vector<SchreierTree *> trees_;
```