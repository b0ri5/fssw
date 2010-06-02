#ifndef INCLUDE_FSSW_PERMUTATIONWORD_H_
#define INCLUDE_FSSW_PERMUTATIONWORD_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <fssw/Permutation.h>
#include <fssw/MapPermutation.h>

#include <string>
#include <utility>
#include <vector>

using std::pair;
using std::vector;

namespace fssw {

class MapPermutation;

/*
 * Static allocation class for MapPermutation objects.
 */
class MapPermutationAllocator {
 public:
  static MapPermutation* new_permutation();
  static void clear_memory();

 private:
  static vector<MapPermutation*> permutations_;
};

/*
 * Single permutation element for the PermutationWord.
 */
struct PermutationPart {
  const MapPermutation *g_ptr;
  bool is_inverse;

  PermutationPart(const MapPermutation *g_ptr, bool is_inverse);
  bool from_string(string s);
};

/*
 * The base permutation object. It only needs to know what an element
 * is mapped to and how to compose itself with another permutation.
 *
 * For now, compose actions do not contract g*g^{-1} and g^{-1}*g to identity;
 * this can be added later, by checking the matching end points of the words:
 * for the two PermutationPart ends g should be the same, is_inverse different.
 */

class PermutationWord : public Permutation {
 public:
  int get_image(int a) const;
  int get_inverse_image(int a) const;
  void clear();

  void compose(const PermutationWord &g);
  void compose_inverse(const PermutationWord &g);
  void compose(const MapPermutation &g);
  void compose_inverse(const MapPermutation &g);

  bool is_identity() const;
  void evaluate(MapPermutation *g_ptr) const;
  bool is_equivalent(const PermutationWord &w) const;

  bool from_string(string s);
  string to_string() const;

  int get_moved_element() const;

 private:
  vector<PermutationPart> permutations_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_PERMUTATIONWORD_H_
