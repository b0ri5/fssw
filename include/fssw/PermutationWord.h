#ifndef INCLUDE_FSSW_PERMUTATIONWORD_H_
#define INCLUDE_FSSW_PERMUTATIONWORD_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/Permutation.h>

namespace fssw {

/*
 * The base permutation object. It only needs to know what an element
 * is mapped to and how to compose itself with another permutation.
 */

class PermutationWord : public Permutation {
 public:
  int get_image(int a) const;
  int get_inverse_image(int a) const;
  void clear();

  void compose(const PermutationWord &g);
  void compose_inverse(const PermutationWord &g);
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_PERMUTATIONWORD_H_
