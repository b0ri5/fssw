#ifndef INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
#define INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <vector>

using std::vector;

namespace fssw {

class MapPermutation;
class PermutationWord;
class SchreierTree;

/*
 * TODO: reevaluate the name of this class and fill in its description
 */

class FundamentalSchreierTrees {
 public:
  // try to un-fix as much of g as possible
  int sift(const PermutationWord &g, PermutationWord *h_ptr);

  // tries to reduce g to identity through the current tree structure
  int strip(const PermutationWord &g, PermutationWord* h_ptr);

  void append_to_base(int b);

  // adds "w" to each schreier tree up until "w" moves a base element
  void add_generator(const PermutationWord &w);

  // wraps MapPermutation in a PermutationWord of length 1
  void add_generator(const MapPermutation &g);

 private:
  vector<int> base_;
  vector<SchreierTree *> trees_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
