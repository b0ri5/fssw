#ifndef INCLUDE_FSSW_MAPPERMUTATION_H_
#define INCLUDE_FSSW_MAPPERMUTATION_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/Permutation.h>

#include <map>
#include <string>

using std::map;
using std::string;

namespace fssw {

/*
 * This is an implementation of Permutation which only stores
 * the mapping for where elements move.
 */

class MapPermutation : public Permutation {
 public:
  int get_image(int a) const;
  int get_inverse_image(int a) const;

  void clear();

  // sets to: (this*g)(a) = g( this(a) )
  // e.g.: this=(0 1 2); g =(0 1), then this*g=(0)(1 2)
  void compose(const MapPermutation &g);

  // composes with g^{-1}
  void compose_inverse(const MapPermutation &g);

  bool is_identity() const;
  bool is_equal(const MapPermutation &g) const;

  // sets the permutation from string s, in disjoint cycle notation,
  // e.g. s="(0 2)(1 4)";
  // in Cartesian representation, s is:
  // s = 0 1 2 3 4
  //     2 4 0 3 1
  // if parsing is unsuccessful return false, set to identity
  bool from_string(string s);

  string to_string() const;

  friend class PermutationWord;

 private:
  map<int, int> images_;
  map<int, int> inverse_images_;

  void set_image(int a, int b);
  void compose_with_mapping(const map<int, int> &mapping);
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_MAPPERMUTATION_H_
