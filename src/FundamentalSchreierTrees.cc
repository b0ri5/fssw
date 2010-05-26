/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/FundamentalSchreierTrees.h>

#include <fssw/SchreierTree.h>

namespace fssw {

FundamentalSchreierTrees::~FundamentalSchreierTrees() {
  // free all of the copies of generators we needed
  for (vector<const MapPermutation *>::iterator
       it = allocated_generators_.begin(); it != allocated_generators_.end();
       ++it) {
    delete *it;
  }

  // free the words
  for (vector<const PermutationWord *>::iterator
       it = allocated_words_.begin(); it != allocated_words_.end();
       ++it) {
    delete *it;
  }

  // free the trees
  for (vector<SchreierTree *>::iterator it = trees_.begin(); it != trees_.end();
       ++it) {
    delete *it;
  }
}

void FundamentalSchreierTrees::add_generator_no_copy(const MapPermutation &g) {
  original_generators_.push_back(&g);

  PermutationWord *w_ptr = new PermutationWord();

  w_ptr->compose(g);
  allocated_words_.push_back(w_ptr);
}

void FundamentalSchreierTrees::add_generator(
    const MapPermutation &g) {
  // copy the generator
  MapPermutation *g_ptr = new MapPermutation();
  g_ptr->from_string(g.to_string());

  // add and keep track of the copy
  add_generator_no_copy(*g_ptr);
  allocated_generators_.push_back(g_ptr);
}

void FundamentalSchreierTrees::append_to_base(int b) {
  // add b to the base
  base_.push_back(b);

  SchreierTree *tree_ptr = new SchreierTree();

  // then to the trees
  tree_ptr->set_root(b);
  trees_.push_back(tree_ptr);
}

void FundamentalSchreierTrees::distribute_generators() {
  for (int i = 0; i < allocated_words_.size(); ++i) {
    distribute_generator(*allocated_words_[i]);
  }
}

void FundamentalSchreierTrees::distribute_generator(const PermutationWord &w) {
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

const SchreierTree* FundamentalSchreierTrees::get_tree(int i) {
  if (i < 0 || i >= trees_.size()) {
    return NULL;
  }

  return trees_[i];
}

int FundamentalSchreierTrees::get_base(int i) {
  if (i < 0 || i >= base_.size()) {
    return -1;
  }

  return base_[i];
}

int FundamentalSchreierTrees::get_base_length() {
  return base_.size();
}



bool FundamentalSchreierTrees::build_trees() {
  bool changed = false;
  for (int i = 0; i < trees_.size(); i++) {
    changed = changed || trees_.at(i)->build_tree();
  }

  return changed;
}

}  // namespace fssw
