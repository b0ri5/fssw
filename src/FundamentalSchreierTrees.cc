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
  for (int i = 0; i < base_.size(); ++i) {
    trees_[i]->add_generator(w);

    int b = base_[i];

    if (w.get_image(b) != b) {
      break;
    }
  }
}

int FundamentalSchreierTrees::strip(const PermutationWord &g,
  PermutationWord* h_ptr) {
  h_ptr->clear();
  h_ptr->compose(g);

  for (int i = 0; i < base_.size(); ++i) {
    int a = h_ptr->get_image(base_.at(i));
    SchreierTree *tree_ptr = trees_.at(i);

    if (!tree_ptr->is_in_orbit(a)) {
      return i;
    }

    PermutationWord path;
    tree_ptr->path_to_root(a, &path);
    h_ptr->compose(path);
  }

  return base_.size();
}

const SchreierTree* FundamentalSchreierTrees::getTree(int i) {
  if (i < 0 || i >= trees_.size()) {
    return NULL;
  }
  return trees_[i];
}

int FundamentalSchreierTrees::getBase(int i) {
  if (i < 0 || i >= base_.size()) {
    return -1;
  }
  return base_[i];
}

void FundamentalSchreierTrees::build_trees() {
  for (int i = 0; i < trees_.size(); i++) {
    trees_.at(i)->build_tree();
  }
}

}  // namespace fssw
