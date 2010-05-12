/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <fssw/SchreierTree.h>

namespace fssw {

void FundamentalSchreierTrees::append_to_base(int b) {
  base_.push_back(b);

  SchreierTree *tree = new SchreierTree();
  tree->set_root(b);
  trees_.push_back(tree);
}

void FundamentalSchreierTrees::add_generator(const PermutationWord &w) {
  int i = 0;

  while (i < base_.size()) {
    trees_[i]->add_generator(w);

    int b = base_[i];

    if (w.get_image(b) != b) {
      break;
    }

    ++i;
  }
}

void FundamentalSchreierTrees::add_generator(const MapPermutation &g) {
  PermutationWord w;
  w.compose(g);

  add_generator(w);
}

int FundamentalSchreierTrees::strip(const PermutationWord &g,
  PermutationWord* h_ptr) {
  h_ptr->clear();
  h_ptr->compose(g);

  int i = 0;

  while (i < base_.size()) {
    int a = h_ptr->get_image(base_.at(i));
    SchreierTree *tree_ptr = trees_.at(i);

    if (!tree_ptr->is_in_orbit(a)) {
      return i;
    } else {
      PermutationWord path;

      tree_ptr->path_to_root(a, &path);
      h_ptr->compose(path);
    }
    ++i;
  }

  return i;
}

void FundamentalSchreierTrees::build_trees() {
  for (int i = 0; i < trees_.size(); i++) {
    trees_.at(i)->build_tree();
  }
}

}  // namespace fssw
