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
 * The base permutation object. It only needs to know what an element
 * is mapped to and how to compose itself with another permutation.
 */

class MapPermutation : public Permutation {
 public:
  int get_image(int a);
  int get_inverse_image(int a);
  void compose(const Permutation &g);

  string input_string(string s);

 private:
  map<int, int> images_;
  map<int, int> inverse_images_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_MAPPERMUTATION_H_
