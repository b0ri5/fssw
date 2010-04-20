#ifndef INCLUDE_FSSW_PERMUTATION_H_
#define INCLUDE_FSSW_PERMUTATION_H_

/*
    Copyright 2010 Greg Tener
    Released under the Lesser General Public License v3.
*/

namespace fssw {

/*
 * The base permutation object. It only needs to know what an element
 * is mapped to and how to compose itself with another permutation.
 */

class Permutation {
 public:
  typedef int element_t;

  virtual element_t get_image(element_t a) = 0;
  virtual void compose(const Permutation &g) = 0;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_PERMUTATION_H_
