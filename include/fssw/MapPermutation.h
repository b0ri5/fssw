#ifndef INCLUDE_FSSW_MAPPERMUTATION_H_
#define INCLUDE_FSSW_MAPPERMUTATION_H_

/*
    Copyright 2010 Greg Tener
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
  element_t get_image(element_t a);
  void compose(const Permutation &g);

  string input_string(string &s);

 private:
  map<int, int> images_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_MAPPERMUTATION_H_
