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

  ~FundamentalSchreierTrees();

  // tries to reduce g to identity through the current tree structure
  int strip(const PermutationWord &g, PermutationWord *h_ptr);

  // simply append, nothing else
  void append_to_base(int b);

  // append and also extend anyone coming from the previous one
  void extend_base(int b);

  // adds "g" as an original generator; makes a copy of "g"
  void add_generator(const MapPermutation &g);
  void add_generator_no_copy(const MapPermutation &g);

  // accessors used in testing
  const SchreierTree *get_tree(int i);
  int get_base(int i);
  int get_base_length();

  // calls build_tree() on each schreier tree, returns true if a tree changed
  bool build_trees();

  // distribute each generator through the trees
  void distribute_generators();

  int schreier_sims(const vector<const PermutationWord> &generators);

  // ensures that base_ is a base for the current group
  bool expand_base(const PermutationWord &g);

 private:
  // adds "w" to each schreier tree up until "w" moves a base element
  void distribute_generator(const PermutationWord &w);

  vector<int> base_;
  vector<SchreierTree *> trees_;
  vector<const MapPermutation *> original_generators_;

  vector<const MapPermutation *> allocated_generators_;
  vector<const PermutationWord *> allocated_words_;
};

}  // namespace fssw

#endif  // INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
