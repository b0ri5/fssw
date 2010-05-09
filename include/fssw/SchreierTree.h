#ifndef INCLUDE_FSSW_SCHREIERTREE_H_
#define INCLUDE_FSSW_SCHREIERTREE_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/PermutationWord.h>

#include <map>

using std::map;

namespace fssw {

/*
 * This is a schreier tree. It keeps track of the orbit of its root
 * element.
 */

class SchreierTree {
 public:
  explicit SchreierTree(int root);

  // adds the generator g to the tree, if we enlarge the orbit (the permutation
  // is used) then return true, otherwise return false
  // the object g should stay in scope as long as this tree does
  bool add_generator(const PermutationWord &g);

  // returns if "a" is minimal in the orbit of root_
  bool is_minimal(int a);

  // returns if "a" is in the orbit of root_
  bool contains(int a);

 private:
  map<int, PermutationWord *> tree_;
  int root_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_SCHREIERTREE_H_
