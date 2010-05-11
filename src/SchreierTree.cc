/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/SchreierTree.h>

namespace fssw {

void SchreierTree::set_root(int root) {
  root_ = root;
}

bool SchreierTree::add_generator(const PermutationWord &g) {
  // start from root_, build edges backwards
  int a = g.get_image(root_);

  if (a == root_) {
    // no edges added
    return false;
  }

  // add tree edges if possible
  while (a != root_) {
    map<int, const PermutationWord*>::iterator tree_it = tree_.find(a);

    // if an existing mapping, do not overwrite
    if (tree_it == tree_.end()) {
      tree_[a] = &g;
    }

    a = g.get_image(a);
  }

  return true;
}

bool SchreierTree::is_in_orbit(int a) {
  return (a == root_) || (tree_.find(a) != tree_.end());
}

bool SchreierTree::path_to_root(int a, PermutationWord *path_ptr) {
  path_ptr->clear();

  if (!is_in_orbit(a)) {
    return false;
  }

  while (a != root_) {
    const PermutationWord *w_ptr = tree_[a];
    path_ptr->compose(*w_ptr);
    a = w_ptr->get_image(a);
  }

  return true;
}

}  // namespace fssw
