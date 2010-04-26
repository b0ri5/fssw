#ifndef INCLUDE_FSSW_SCHREIERTREE_H_
#define INCLUDE_FSSW_SCHREIERTREE_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

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

  bool is_minimal_in_orbit(int a);

 private:
  map<int, Permutation *> tree_;
  int root_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_SCHREIERTREE_H_
