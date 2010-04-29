#ifndef INCLUDE_FSSW_PERMUTATION_H_
#define INCLUDE_FSSW_PERMUTATION_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

namespace fssw {

/*
 * The base permutation object. It only needs to know what an element
 * is mapped to and how to compose itself with another permutation.
 */

class Permutation {
 public:
  virtual int get_image(int a) = 0;
  virtual int get_inverse_image(int a) = 0;
  virtual void compose(const Permutation &g) = 0;
  virtual void compose_inverse(const Permutation &g) = 0;
  virtual void clear() = 0;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_PERMUTATION_H_
