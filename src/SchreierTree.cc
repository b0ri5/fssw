/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/SchreierTree.h>

#include <deque>

using std::deque;

namespace fssw {

bool OrbitIterator::has_next() {
  return it_ != tree_ptr_->end();
}

OrbitIterator &OrbitIterator::operator++() {
  ++it_;
  return *this;
}

int OrbitIterator::operator*() const {
  return it_->first;
}

void SchreierTree::set_root(int root) {
  root_ = root;
}

void SchreierTree::expand_orbit(
    const vector<const PermutationWord *> &generators,
    deque<int> *new_elements_ptr) {

  // iterate over the elements in the orbit
  for (SchreierTree::iterator orbit_it = get_orbit_iterator();
      orbit_it.has_next(); ++orbit_it) {
    int a = *orbit_it;

    // now iterate over the generators
    for (int i = 0; i < generators.size(); i++) {
      int inverse_image = generators[i]->get_image(a);

      if (!is_in_orbit(inverse_image)) {
        tree_[inverse_image] = generators[i];
      }
    }

    // couldn't figure out how to get this to compile... kept getting const
    // issues
    /*for (vector<const PermutationWord *>::const_iterator gens_it =
             generators.begin(); gens_it != generators.end();
             ++gens_it) {
      //int image = gens_it->get_image(a);
    }*/
  }
}


bool SchreierTree::add_generator(const PermutationWord &g) {
  deque<int> new_elements;

  return false;
/*
  // find the "new" elements in the orbit provided by g
  for (SchreierTree::iterator it = get_orbit_iterator(); it.has_next(); ++it) {
    int a = *it;
    int inverse_image = g.get_inverse_image(a);

    // add new elements in the cycle containing "a" to the tree
    while (inverse_image != a) {
      if (!is_in_orbit(inverse_image)) {
        tree_[inverse_image] = &g;
        new_elements.push_back(inverse_image);
      }
      inverse_image = g.get_inverse_image(inverse_image);
    }
  }

  generators_.push_back(&g);

  // now expand the orbit from new_elements
  while (new_elements.size() > 0) {
    int a = new_elements.front();
    new_elements.pop_front();

    for (vector<const PermutationWord *>::iterator generators_it = \
         generators_.begin(); generators_it != generators_.end();
         ++generators_it)
    {
      for (SchreierTree::iterator it = get_orbit_iterator(); it.has_next();
              ++it) {
        int a = *it;
        int inverse_image = g.get_inverse_image(a);

        while (inverse_image != a) {
          if (!is_in_orbit(inverse_image)) {
            tree_[inverse_image] =
          }
        }
      }
    }
  }*/
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

OrbitIterator SchreierTree::get_orbit_iterator() {
  OrbitIterator it;

  it.tree_ptr_ = &tree_;
  it.it_ = tree_.begin();

  return it;
}

}  // namespace fssw
