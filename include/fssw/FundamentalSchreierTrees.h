#ifndef INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
#define INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_

/*
    Copyright 2010 Greg Tener and Elena Erbiceanu
    Released under the Lesser General Public License v3.
*/

#include <vector>
#include <string>

using std::vector;
using std::string;

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
  int strip(const PermutationWord &g, PermutationWord *h_ptr) const;

  // append b to the base (add a new tree);
  // also copies the appropriate generators in the previous tree to the new one
  void append_to_base(int b);

  // append and also extend anyone coming from the previous one
  void extend_base(int b);

  // adds "g" as an original generator; makes a copy of "g"
  void add_generator(const MapPermutation &g);
  void add_generator_no_copy(const MapPermutation &g);

  // accessors used in testing
  const SchreierTree *get_tree(int i) const;
  int get_base(int i) const;
  void get_base(vector<int> *v_ptr) const;
  int get_base_length() const;
  const PermutationWord *get_original_word(int i) const;
  int get_original_generators_length() const;

  // for testing purposes, output as a string
  string str() const;

  // calls build_tree() on each schreier tree, returns true if a tree changed
  bool build_trees();

  int schreier_sims(const vector<const PermutationWord> &generators);

  // ensures that base_ is a base for the current group
  bool expand_base(const PermutationWord &g);

  // appends elements to the base until each generator moves the base
  bool ensure_each_generator_moves_base();

 private:
  // adds "w" to each schreier tree up until "w" moves a base element
  void distribute_generator(const PermutationWord &w);

  vector<int> base_;
  vector<SchreierTree *> trees_;
  vector<const MapPermutation *> original_generators_;
  vector<const PermutationWord *> original_words_;

  vector<const MapPermutation *> allocated_generators_;
  vector<const PermutationWord *> allocated_generator_words_;
};

template <typename T>
static void delete_vector_ptrs(const vector<T *> &v) {
  for (typename vector<T *>::const_iterator
       it = v.begin(); it != v.end();
       ++it) {
    delete *it;
  }
}

}  // namespace fssw

#endif  // INCLUDE_FSSW_FUNDAMENTALSCHREIERTREES_H_
