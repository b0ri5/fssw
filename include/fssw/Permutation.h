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
 * The permutation acts on the nonnegative integers {0, ...}. The variables
 * "a" and "b" will be used to denote nonnegative integers and the variables
 * "g" and "h" will be used to denote permutations
 * (i.e. they're group elements).
 */

class Permutation {
 public:
  // returns the image of element a: a^{this}
  virtual int get_image(int a) = 0;

  // returns a^{this^{-1}}
  virtual int get_inverse_image(int a) = 0;

  // sets to: (this*g)(a) = g( this(a) )
  // e.g.: this=(0 1 2); g =(0 1), then this*g=(0)(1 2)
  virtual void compose(const Permutation &g) = 0;

  // composes with g^{-1}
  virtual void compose_inverse(const Permutation &g) = 0;

  // sets to identity
  virtual void clear() = 0;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_PERMUTATION_H_
