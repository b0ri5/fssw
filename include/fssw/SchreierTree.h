#ifndef INCLUDE_FSSW_SCHREIERTREE_H_
#define INCLUDE_FSSW_SCHREIERTREE_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/PermutationWord.h>

#include <map>
#include <deque>
#include <vector>
#include <string>

using std::map;
using std::deque;
using std::vector;
using std::string;

namespace fssw {

/*
 * A simple iterator over the root elements of the schreier tree.
 * Wraps the tree_'s iterator and just returns the first's elements.
 */
class OrbitIterator {
 public:
  friend class SchreierTree;

  OrbitIterator();
  OrbitIterator(map<int, const PermutationWord *> *tree_ptr, int root);

  // returns true if there's more to be iterated over
  bool not_at_end();

  // appends the value "a" to the iterator, this is useful when changing the
  // schreier tree during iteration. This only appends elements if they are
  // smaller than where we are in the iterator already, then consumes them
  // before incrementing it_
  void append(int a);

  // advances iterator
  OrbitIterator &operator++();

  // returns the current orbit element
  int operator*() const;
 private:
  map<int, const PermutationWord *> *tree_ptr_;
  map<int, const PermutationWord *>::const_iterator tree_it_;

  // a list of elements smaller than where we currently are
  deque<int> smaller_elements_;
  // lets us know if the queue should be used or not
  bool use_queue_;
};

/*
 * This is a schreier tree. It keeps track of the orbit of its root
 * element.
 */

class SchreierTree {
 public:
  // sets the root of the tree; this should be called right after creation
  void set_root(int root);
  int get_root();

  // adds the generator g to the tree, if we enlarge the orbit (the permutation
  // is used) then return true, otherwise return false
  // the object g should stay in scope as long as this tree does
  void add_generator(const PermutationWord &g);

  // builds the tree, returns true if the tree changes
  bool build_tree();

  // returns if "a" is minimal in the orbit of root_
  bool is_minimal(int a);

  // returns if "a" is in the orbit of root_
  bool is_in_orbit(int a);

  // returns false if a is not in the orbit
  bool path_to_root(int a, PermutationWord *path_ptr);
  bool path_from_root(int a, PermutationWord *path_ptr);

  typedef OrbitIterator iterator;
  OrbitIterator get_orbit_iterator();

  bool has_generator(const MapPermutation &g) const;

  int size() const;

  string to_string() const;

  friend class FundamentalSchreierTrees;

 private:

  map<int, const PermutationWord *> tree_;
  int root_;

  vector<const PermutationWord *> generators_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_SCHREIERTREE_H_
