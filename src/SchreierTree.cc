/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/SchreierTree.h>

#include <deque>
#include <sstream>

using std::deque;
using std::stringstream;

namespace fssw {

OrbitIterator::OrbitIterator() {
}

OrbitIterator::OrbitIterator(map<int, const PermutationWord *> *tree_ptr,
    int root) {
  tree_ptr_ = tree_ptr;
  tree_it_ = tree_ptr->begin();
  append(root);
}

bool OrbitIterator::has_next() {
  return !smaller_elements_.empty() || tree_it_ != tree_ptr_->end();
}

void OrbitIterator::append(int a) {
  if (tree_it_ == tree_ptr_->end() || a < tree_it_->first) {
    smaller_elements_.push_back(a);
  }
}

OrbitIterator &OrbitIterator::operator++() {
  if (!smaller_elements_.empty()) {
    smaller_elements_.pop_front();
  } else {
    ++tree_it_;
  }

  return *this;
}

int OrbitIterator::operator*() const {
  if (!smaller_elements_.empty()) {
    return smaller_elements_.front();
  }

  return tree_it_->first;
}

void SchreierTree::set_root(int root) {
  root_ = root;
}

int SchreierTree::get_root() {
  return root_;
}

bool SchreierTree::build_tree() {
  bool tree_changed = false;

  // iterate over the elements in the orbit
  for (SchreierTree::iterator orbit_it = get_orbit_iterator();
      orbit_it.has_next(); ++orbit_it) {
    int a = *orbit_it;

    // iterate over the generators
    for (vector<const PermutationWord *>::iterator gens_it =
             generators_.begin(); gens_it != generators_.end();
             ++gens_it) {
      int inverse_image = (*gens_it)->get_inverse_image(a);

      // if we found something not in the orbit
      if (!is_in_orbit(inverse_image)) {
        tree_changed = true;
        tree_[inverse_image] = *gens_it;
        orbit_it.append(inverse_image);  // be sure to tell the orbit iterator
      }
    }
  }

  return tree_changed;
}


void SchreierTree::add_generator(const PermutationWord &g) {
  generators_.push_back(&g);
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

bool SchreierTree::path_from_root(int a, PermutationWord *path_ptr) {
  PermutationWord w;
  bool rv = path_to_root(a, &w);
  path_ptr->compose_inverse(w);

  return rv;
}

OrbitIterator SchreierTree::get_orbit_iterator() {
  return OrbitIterator(&tree_, root_);
}

bool SchreierTree::has_generator(const MapPermutation &g) const {
  // search for the generator it that exact form, by matching strings
  string g_str = g.to_string();

  for (vector<const PermutationWord*>::const_iterator gen_it =
    generators_.begin(); gen_it != generators_.end(); ++gen_it) {
    if ((*gen_it)->to_string() == g_str) {
      return true;
    }
  }
  return false;
}

string SchreierTree::to_string() const {
  stringstream ss;

  ss << "root: " << root_ << "\n";

  ss << "generators: ";
  for (int i = 0; i < generators_.size(); ++i) {
    ss << generators_[i]->to_string() << " ";
  }
  ss << "\n";

  ss << "tree: " << "{ ";
  for (map<int, const PermutationWord *>::const_iterator it = tree_.begin();
       it != tree_.end(); ++it) {
    ss << it->first << ":" << it->second->to_evaluated_string() << " ";
  }
  ss << "}";

  return ss.str();
}

}  // namespace fssw
